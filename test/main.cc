#include <iostream>
#include <vector>
#include <boost/thread.hpp>

#include "lib/network/websocket/client/websocket.h"
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
};

std::string calcResponseSecretKey(const std::string& secret_key)
{
    return client::HandshakeHelper::calcResponseSecret(secret_key);
}

void validate_hand_shake_response(client::HandShakeResponse::ptr h_res)
{
    if (!h_res) {
        std::printf("1\n");
        throw std::exception();
    }

    if (h_res->status_code != 101) {
        std::printf("2\n");
        throw std::exception();
    }

    if (h_res->upgrade != "websocket") {
        std::printf("3\n");
        throw std::exception();
    }

    if (h_res->connection != "Upgrade") {
        std::printf("4\n");
        throw std::exception();
    }

    std::string secret_key = calcResponseSecretKey(SECRET_KEY);

    if (h_res->secret_accept != secret_key) {
        std::printf("5\n");
        throw std::exception();
    }

    if (h_res->protocol == "" || h_res->protocol != PROTOCOL_NAME) {
        std::printf("6\n");
        throw std::exception();
    }
}

void send_packet(client::Websocket::ptr so, const std::vector<char>& v)
{
    char mask_key[4] = {0x1, 0x2, 0x3, 0x4};

    PacketData::ptr pd = new PacketData;
    pd->fin = true;
    pd->cont_frame = false;

    pd->packet_type = PACKET_TYPE_BINARY;
    pd->data.insert(pd->data.end(), v.begin(), v.end());

    so->write(pd, mask_key, TIMEOUT_MILLIS, WRITE_RETRY);
}

void receive_packet(client::Websocket::ptr so, int timeout)
{
    PacketData::ptr pd;

    try {
        pd = so->read(timeout, READ_RETRY);
    } catch (...) {}

    if (pd && pd->data.size() > 0) {
        char* buf = new char[pd->data.size()];
        for (int i = 0; i < pd->data.size(); i++) {
            buf[i] = pd->data[i];
        }

        //
        std::printf("=== response data ===\n");
        std::printf("data size: %ld\n", pd->data.size());

        AAA a;
        a.ParseFromArray(buf, pd->data.size());
        std::printf("str=%s\n", a.str().c_str());
        std::printf("x=%d\n", a.x());

        delete[] buf;

        std::printf("=====================\n\n");
    }
}

void createUiid(const std::string& value, UiidPData& u_data,
    std::vector<char>& v)
{
    const int OP_CODE = 1;
    const int OP_CODE_SIZE = 4;
    const int INT_SIZE = 4;

    u_data.set_value(value);

    int a_size = u_data.ByteSize();
    char* buf = new char[a_size];

    memset(buf, 0, a_size + OP_CODE_SIZE + INT_SIZE);
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

int main(int argc, char** argv)
{
/*
    {
        AAA a1;
        a1.set_str("aaaxxxxxxxx");
        a1.set_x(10);

        int a_size = a1.ByteSize();
        char* bi_aaa = new char[a_size];
        memset(bi_aaa, 0, a_size);

        a1.SerializeToArray(bi_aaa, a_size);

        //
        AAA a2;
        a2.ParseFromArray(bi_aaa, a_size);
        std::printf("str=%s\n", a2.str().c_str());
        std::printf("x=%d\n", a2.x());

        std::printf("a2 size=%d\n", a2.ByteSize());

        delete[] bi_aaa;
    }
*/

    try {
        const int DEFAULT_PORT = 9000;
        int port = DEFAULT_PORT;

        if (argc > 2) {
            std::string s = argv[1];

            if (s == "-p") {
                port = atoi(argv[2]);

                if (port == 0) {
                    port = DEFAULT_PORT;
                }
            }
        }

        //
        client::HandShakeRequest::ptr h_req = new client::HandShakeRequest;
        h_req->path = "/jikken3";
        h_req->secret_key = client::HandshakeHelper::createSecret();

        h_req->protocols.insert(PROTOCOL_NAME);

        client::Websocket::ptr so = 
            new client::Websocket("0.0.0.0", port);
        so->connect(60 * 1000);

        client::HandShakeResponse::ptr h_res = 
            so->handShake(h_req, 60 * 1000, HANDSHAKE_RETRY);
        validate_hand_shake_response(h_res);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1));

        {
            std::vector<char> v;
            /*
            AAA a;
            a.set_str("aaaaabbbbbccccc");
            a.set_x(1);
            */
            UiidPData u_data;
            createUiid("aaaaabbbbbccccc0", u_data, v);

            //
            send_packet(so, v);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 5000);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 1);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        {
            std::vector<char> v;
            UiidPData u_data;
            createUiid("ddddeeeeffffgggg", u_data, v);

            //
            send_packet(so, v);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 50);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 1);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        {
            std::vector<char> v;
            UiidPData u_data;
            createUiid("0123456789abcdef", u_data, v);

            //
            send_packet(so, v);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, TIMEOUT_MILLIS);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        {
            std::vector<char> v;
            UiidPData u_data;
            createUiid("aaa-111-vvv-666", u_data, v);

            //
            send_packet(so, v);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 600);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 1000);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        sleep(5);

        so->close();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

