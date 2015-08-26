#include "user_client.h"

#include "common_object.h"
#include "router/command_dispatcher.h"

#include "atomic/atomic_operator.hpp"

#include "network/websocket/packet.h"
#include "log/logger.h"

UserClient::UserClient(server::WebsocketContext* s) : 
    session(s),
    first_process(true),
    read_cnt(0),
    write_cnt(0)
{
    //
}

UserClient::~UserClient()
{
    if (session) {
        session->destroyAsync();
    }
}

void UserClient::update() const
{
    //
}

void UserClient::close() const
{
    session->close();
}

void UserClient::write(PacketData::ptr pd,
    server::SendCallback send_callback) const
{
    if (!session->isOpen()) {
        return;
    }
    
    AtomicOperator<size_t>::increment(&write_cnt);
    session->write(pd, send_callback);
}

void UserClient::onStart() const
{
    AtomicOperator<size_t>::increment(&read_cnt);
    AtomicOperator<bool>::lock_test_and_set(&first_process, false);
    
    session->read();
}

void UserClient::onReceive(PacketData::ptr r_pd) const
{
    if (r_pd->packet_type == PACKET_TYPE_BINARY) {
        CommandDispatcher::bulkDispatch(getKey(), r_pd->data);
    } else {
        Logger::log("user client: warning packet type=%d, data size=%ld\n",
            r_pd->packet_type, r_pd->data.size());
    }
}

void UserClient::onReceiveFinish(boost::system::error_code ec) const
{
    if (!ec && session->isOpen()) {
        session->read();
    } else {
        AtomicOperator<size_t>::decrement(&read_cnt);
    }
}

void UserClient::onSendFinish(boost::system::error_code ec) const
{
    AtomicOperator<size_t>::decrement(&write_cnt);
}

void UserClient::onError(Operation operation, 
    boost::system::error_code ec) const
{
    auto router = CommonObject::getInstance()->getErrorHandleRouter();
    auto callback = router->getCallback(ec.value());

    if (callback) {
        callback(this);
    } else {
        if (session->isOpen()) {
            session->close();
        }
    }
}

bool UserClient::isActive() const
{
    return first_process || read_cnt > 0 || write_cnt > 0;
}

long UserClient::getKey() const
{
    return session->getKey();
}
