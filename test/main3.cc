#include <iostream>
#include <vector>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "network/websocket/client/websocket_async.h"
#include "network/websocket/client/handshake_helper.h"

//#include "jikken.pb.h"
//#include "uiid.pb.h"

#include "uiid_generated.h"

namespace {
    const int TIMEOUT_MILLIS = 5000;

    const int HANDSHAKE_RETRY = 3;
    const int WRITE_RETRY = 1;
    const int READ_RETRY = 3;

    const char* PROTOCOL_NAME = "realtime_battle";

    bool end_flag = false;

    flatbuffers::FlatBufferBuilder fbb;
    std::vector<char> verify_file;
};

void createUiid(const std::string& value, std::vector<char>& v)
{
    auto fb_uiid = fbb.CreateString(value);

    auto data = CreateUiidData(fbb, fb_uiid);
    fbb.Finish(data);

    const int OP_CODE = 1;
    const int OP_CODE_SIZE = 4;
    const int INT_SIZE = 4;

    auto a_size = fbb.GetSize();
    auto buf = (const char*)fbb.GetBufferPointer();

    const int DATA_HEAD = OP_CODE_SIZE + INT_SIZE;

    //
    char op_code[4] = {0};
    char size_buf[4] = {0};

    op_code[0] = (OP_CODE >> 24) & 255;
    op_code[1] = (OP_CODE >> 16) & 255;
    op_code[2] = (OP_CODE >> 8) & 255;
    op_code[3] = OP_CODE & 255;

    size_buf[0] = (a_size >> 24) & 255;
    size_buf[1] = (a_size >> 16) & 255;
    size_buf[2] = (a_size >> 8) & 255;
    size_buf[3] = a_size & 255;

    //
    for (int i = 0; i < 4; i++) {
        v.push_back(op_code[i]);
    }

    for (int i = 0; i < 4; i++) {
        v.push_back(size_buf[i]);
    }

    for (int i = 0; i < a_size; i++) {
        v.push_back(buf[i]);
    }

    fbb.Clear();
}

class WebsocketDelegateImpl : public client::WebsocketDelegate
{
public:
    WebsocketDelegateImpl()
    {
        send_count = 0;
        receive_count = 0;
        max_send = 5000;
    }

    virtual void onStart(client::WebsocketAsync* ws)
    {
//        std::printf("start\n");

        {
            std::vector<char> v;
            createUiid("aaaaabbbbbccccc0", v);

            char mask_key[4] = {0x1, 0x2, 0x3, 0x4};

            PacketData::ptr pd = new PacketData;
            pd->fin = true;
            pd->cont_frame = false;

            pd->packet_type = PACKET_TYPE_BINARY;
            pd->data.insert(pd->data.end(), v.begin(), v.end());

            ws->write(pd, mask_key, [](boost::system::error_code ec) {
//                std::printf("write!\n");
            });

            send_count++;

            ws->read();
        }
    }

    virtual void onReceive(client::WebsocketAsync* ws,
        PacketData::ptr pd)
    {
//        std::printf("receive finish\n");

        {
            if (pd && pd->data.size() > 0) {
//                char* buf = new char[pd->data.size()];
//                for (int i = 0; i < pd->data.size(); i++) {
//                    buf[i] = pd->data[i];
//                }

//                delete[] buf;
//                std::printf("=====================\n\n");
            }

            receive_count++;

            if (receive_count < max_send) {
                ws->read();
            } else {
                std::printf("==== end ====\n");
//                end_flag = true;
            }

//            std::printf("receive count: %d\n", receive_count);
        }
    }

    virtual void onReceiveFinish(client::WebsocketAsync* ws,
        boost::system::error_code ec)
    {
//        std::printf("receive finish\n");
    }

    virtual void onSendFinish(client::WebsocketAsync* ws,
        boost::system::error_code ec)
    {
//        std::printf("send finish!\n");

        if (send_count < max_send) {
            std::vector<char> v;
            createUiid("aaaaabbbbbccccc0", v);

            char mask_key[4] = {0x1, 0x2, 0x3, 0x4};

            PacketData::ptr pd = new PacketData;
            pd->fin = true;
            pd->cont_frame = false;

            pd->packet_type = PACKET_TYPE_BINARY;
            pd->data.insert(pd->data.end(), v.begin(), v.end());

            ws->write(pd, mask_key, [](boost::system::error_code ec) {
//                std::printf("write!\n");
            });

            send_count++;
        }
    }

    virtual void onError(client::WebsocketAsync* ws,
        Operation operation, boost::system::error_code ec)
    {
        std::printf("error: %s\n", ec.message().c_str());
        end_flag = true;
    }

private:
    int max_send;
    int send_count;
    int receive_count;
};

bool VerifyCertificate(bool preverified,
    boost::asio::ssl::verify_context& ctx)
{
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

//    std::cout << "Verifying " << subject_name << "\n";

    return preverified;
}

int main(int argc, char** argv)
{
    try {
        {
            FILE* fp = 
                fopen("/Users/ito/qtproj/realtime_server/key/server.crt", "r");

            if (fp) {
                int c = 0;
                while ((c = fgetc(fp)) != EOF) {
                    verify_file.push_back(c);
                }

                fclose(fp);
            }
        }

        boost::asio::io_service ios;
        boost::asio::ssl::context ssl_context(ios,
            boost::asio::ssl::context::tlsv1);

        ssl_context.set_verify_mode(boost::asio::ssl::context::verify_peer);
        ssl_context.set_verify_callback(boost::bind(
            VerifyCertificate, _1, _2));
        ssl_context.add_certificate_authority(
            boost::asio::buffer(verify_file));

        std::vector<client::WebsocketAsync*> v;

        auto del = new WebsocketDelegateImpl;

        for (int i = 0; i < 100; i++) {
            auto ws = client::WebsocketAsync::createSSL(ios, 
                ssl_context, "::1", 9000, 600 * 1000);
            ws->setDelegate(del);

            //
            client::HandShakeRequest::ptr h_req = new client::HandShakeRequest;
            h_req->path = "/jikken3";
            h_req->secret_key = client::HandshakeHelper::createSecret();

            h_req->protocols.insert(PROTOCOL_NAME);

            ws->connect(h_req);
            v.push_back(ws);
        }

        while (!end_flag) {
            ios.run_one();
        }

        ios.poll();
        ios.stop();

//        sleep(1);

        for (auto& vv: v) {
            delete vv;
        }

        delete del;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

