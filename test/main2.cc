#include <iostream>
#include <vector>
#include <boost/thread.hpp>

#include "lib/network/protocol/websocket_client.h"
#include "jikken.pb.h"

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
    // todo
    return "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
}

void validate_hand_shake_response(HandShakeResponse::ptr h_res)
{
    if (!h_res) {
        throw std::exception();
    }

    if (h_res->status_code != 101) {
        throw std::exception();
    }

    if (h_res->upgrade != "websocket") {
        throw std::exception();
    }

    if (h_res->connection != "Upgrade") {
        throw std::exception();
    }

    std::string secret_key = calcResponseSecretKey(SECRET_KEY);

    if (h_res->secret_accept != secret_key) {
        throw std::exception();
    }

    if (h_res->protocol == "" || h_res->protocol != PROTOCOL_NAME) {
        throw std::exception();
    }
}

void send_packet(WebsocketClient::ptr so, const std::string& s)
{
    char mask_key[4] = {0x1, 0x2, 0x3, 0x4};

    PacketData::ptr pd = new PacketData;
    pd->fin = true;
    pd->cont_frame = false;

    pd->packet_type = PACKET_TYPE_TEXT;
    pd->data.insert(pd->data.end(), s.begin(), s.end());

    so->write(pd, mask_key, TIMEOUT_MILLIS, WRITE_RETRY);
}

void receive_packet(WebsocketClient::ptr so, int timeout)
{
    PacketData::ptr pd;

    try {
        pd = so->read(timeout, READ_RETRY);
    } catch (...) {}

    if (pd) {
        for (int i = 0; i < pd->data.size(); i++) {
            std::cout << pd->data[i];
        }
        std::cout << std::endl;
    }
}

int main()
{
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

    try {
        HandShakeRequest::ptr h_req = new HandShakeRequest;
        h_req->path = "/jikken2";
        h_req->secret_key = "dGhlIHNhbXBsZSBub25jZQ==";

        h_req->protocols.insert(PROTOCOL_NAME);

        WebsocketClient::ptr so = new WebsocketClient("0.0.0.0", 9000);
        so->connect(60 * 1000);

        HandShakeResponse::ptr h_res = so->handShake(h_req, 60 * 1000, HANDSHAKE_RETRY);
        validate_hand_shake_response(h_res);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1));

        {
            send_packet(so, "aaaaaaaaaabbbbbbbbbbcccccccccc");
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 5000);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 1);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        {
            send_packet(so, "aaaaaaaaaabbbbbbbbbbccccccccccdddddddddd");
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 50);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, 1);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        {
            std::string p = "aaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccdddddddddd";

            send_packet(so, p);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

            receive_packet(so, TIMEOUT_MILLIS);
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

        {
            std::string tmp = "aaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccddddddddddaaaaaaaaaabbbbbbbbbbccccccccccdddddddddd";
            std::string p;
            for (int i = 0; i < 10; i++) {
                p += tmp;
            }

            send_packet(so, p);
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

