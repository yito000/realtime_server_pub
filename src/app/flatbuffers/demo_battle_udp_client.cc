#include "demo_battle_udp.h"

#include "flatbuffers_socket_manager.h"
#include "network/udp_socket_proxy.h"

namespace DemoBattle {
void notify_attack_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::AttackDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = player_id;
    auto p3 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyAttackAction(fbb, p1, p2, p3);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 1;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_buff_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::BuffDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = player_id;
    auto p3 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyBuffAction(fbb, p1, p2, p3);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 2;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_debuff_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::DebuffDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = player_id;
    auto p3 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyDebuffAction(fbb, p1, p2, p3);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 3;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_heal_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::HealDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = player_id;
    auto p3 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyHealAction(fbb, p1, p2, p3);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 4;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_meditation_action_udp(int seq_id, int player_id, const flatbuffers::Vector<const DemoBattle::MeditationDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = player_id;
    auto p3 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyMeditationAction(fbb, p1, p2, p3);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 5;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_start_phase_udp(int seq_id, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;

    auto data = DemoBattle::CreateNotifyStartPhase(fbb, p1);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 100;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_end_phase_udp(int seq_id, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;

    auto data = DemoBattle::CreateNotifyEndPhase(fbb, p1);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 101;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};

namespace DemoBattle {
void notify_leave_player_udp(int player_id, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = player_id;

    auto data = DemoBattle::CreateNotifyLeavePlayer(fbb, p1);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();
    auto udp_client = socket_manager->getUdpSocket();

    if (udp_client) {
        int op_code = 102;

        unsigned char s_op_code[4] = {0};
        unsigned char size_buf[4] = {0};

        s_op_code[0] = (op_code >> 24) & 255;
        s_op_code[1] = (op_code >> 16) & 255;
        s_op_code[2] = (op_code >> 8) & 255;
        s_op_code[3] = op_code & 255;

        size_buf[0] = (buf_size >> 24) & 255;
        size_buf[1] = (buf_size >> 16) & 255;
        size_buf[2] = (buf_size >> 8) & 255;
        size_buf[3] = buf_size & 255;

        //
        ByteBuffer packet;
        packet.reserve(4 + 4 + buf_size);
        for (int i = 0; i < 4; i++) {
            packet.push_back(s_op_code[i]);
        }

        for (int i = 0; i < 4; i++) {
            packet.push_back(size_buf[i]);
        }

        for (int i = 0; i < buf_size; i++) {
            packet.push_back(buf[i]);
        }

        udp_client->send(packet);
    }
}
};
