#include "demo_battle.h"

#include "notify_attack_action_generated.h"
#include "notify_buff_action_generated.h"
#include "notify_debuff_action_generated.h"
#include "notify_heal_action_generated.h"
#include "notify_meditation_action_generated.h"
#include "notify_start_phase_generated.h"
#include "notify_end_phase_generated.h"

#include "flatbuffers_socket_manager.h"
#include "network/websocket/ws_packet_data_helper.h"

namespace DemoBattle {
void notify_attack_action(int seq_id, const flatbuffers::Vector<const DemoBattle::AttackDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyAttackAction(fbb, p1, p2);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(1, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};

namespace DemoBattle {
void notify_buff_action(int seq_id, const flatbuffers::Vector<const DemoBattle::BuffDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyBuffAction(fbb, p1, p2);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(2, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};

namespace DemoBattle {
void notify_debuff_action(int seq_id, const flatbuffers::Vector<const DemoBattle::DebuffDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyDebuffAction(fbb, p1, p2);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(3, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};

namespace DemoBattle {
void notify_heal_action(int seq_id, const flatbuffers::Vector<const DemoBattle::HealDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyHealAction(fbb, p1, p2);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(4, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};

namespace DemoBattle {
void notify_meditation_action(int seq_id, const flatbuffers::Vector<const DemoBattle::MeditationDetail *>* actions, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;
    auto p2 = fbb.CreateVectorOfStructs(actions->Get(0), actions->size());

    auto data = DemoBattle::CreateNotifyMeditationAction(fbb, p1, p2);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(5, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};

namespace DemoBattle {
void notify_start_phase(int seq_id, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;

    auto data = DemoBattle::CreateNotifyStartPhase(fbb, p1);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(100, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};

namespace DemoBattle {
void notify_end_phase(int seq_id, long actor_key)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto p1 = seq_id;

    auto data = DemoBattle::CreateNotifyEndPhase(fbb, p1);
    fbb.Finish(data);

    auto buf_size = fbb.GetSize();
    auto buf = (const unsigned char*)fbb.GetBufferPointer();

    //
    auto socket_manager = FlatbuffersSocketManager::getInstance();

    auto packet = WsPacketDataHelper::buildPacket(101, buf, buf_size);
    packet->packet_type = PACKET_TYPE_BINARY;

    socket_manager->writeTCPSocket(packet, actor_key);
}
};
