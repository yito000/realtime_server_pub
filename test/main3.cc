#include <iostream>
#include <vector>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "lib/network/websocket/client/websocket_async.h"
#include "lib/network/websocket/client/handshake_helper.h"

#include "jikken.pb.h"
#include "uiid.pb.h"

namespace {
    const int TIMEOUT_MILLIS = 5000;

    const int HANDSHAKE_RETRY = 3;
    const int WRITE_RETRY = 1;
    const int READ_RETRY = 3;

    const char* SECRET_KEY = "dGhlIHNhbXBsZSBub25jZQ==";
    const char* PROTOCOL_NAME = "realtime_battle";

    bool end_flag = false;
};

void createUiid(const std::string& value, UiidPData& u_data,
    std::vector<char>& v)
{
    const int OP_CODE = 1;
    const int OP_CODE_SIZE = 4;
    const int INT_SIZE = 4;

    u_data.set_value(value);

    int a_size = u_data.ByteSize();
    char* buf = new char[a_size];

    memset(buf, 0, a_size);
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

    u_data.SerializeToArray(buf, a_size);
    for (int i = 0; i < a_size; i++) {
        v.push_back(buf[i]);
    }

    delete[] buf;
}

class WebsocketDelegateImpl : public client::WebsocketDelegate
{
public:
    WebsocketDelegateImpl()
    {
        send_count = 0;
        receive_count = 0;
        max_send = 500;
    }

    virtual void onStart(client::WebsocketAsync* ws)
    {
//        std::printf("start\n");

        {
            std::vector<char> v;
            UiidPData u_data;
            createUiid("aaaaabbbbbccccc0", u_data, v);

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
                char* buf = new char[pd->data.size()];
                for (int i = 0; i < pd->data.size(); i++) {
                    buf[i] = pd->data[i];
                }

                //
//                std::printf("=== response data ===\n");
//                std::printf("data size: %ld\n", pd->data.size());

                AAA a;
                a.ParseFromArray(buf, pd->data.size());
//                std::printf("str=%s\n", a.str().c_str());
//                std::printf("x=%d\n", a.x());

                delete[] buf;
//                std::printf("=====================\n\n");
            }

            receive_count++;

            if (receive_count < max_send) {
                ws->read();
            } else {
//                std::printf("==== end ====\n");
//                end_flag = true;
            }

//            std::printf("receive count: %d\n", receive_count);
        }
    }

    virtual void onReceiveFinish(client::WebsocketAsync* ws)
    {
//        std::printf("receive finish\n");
    }

    virtual void onSendFinish(client::WebsocketAsync* ws)
    {
//        std::printf("send finish!\n");

        if (send_count < max_send) {
            std::vector<char> v;
            UiidPData u_data;
            createUiid("aaaaabbbbbccccc0", u_data, v);

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
        boost::system::error_code ec)
    {
        std::printf("error: %s\n", ec.message().c_str());
//        end_flag = true;
    }

private:
    int max_send;
    int send_count;
    int receive_count;
};

int main(int argc, char** argv)
{
    try {
        boost::asio::io_service ios;

        std::vector<client::WebsocketAsync::ptr> v;

        for (int i = 0; i < 1000; i++) {
            auto del = new WebsocketDelegateImpl;
            auto ws = new client::WebsocketAsync(ios, 
                "0.0.0.0", 9000, 600 * 1000, 3);
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
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

