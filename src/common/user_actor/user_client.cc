#include "user_client.h"

#include "common_object.h"
#include "protobuf/pb_command_dispatcher.h"

#include "network/websocket/packet.h"
#include "log/logger.h"

UserClient::UserClient(server::WebsocketContext* s) : session(s)
{
    //
}

UserClient::~UserClient()
{
    session->destroyAsync();
}

void UserClient::update() const
{
    //
}

void UserClient::write(PacketData::ptr pd,
    server::SendCallback send_callback) const
{
    session->write(pd, send_callback);
}

void UserClient::onStart() const
{
    session->read();
}

void UserClient::onReceive(PacketData::ptr r_pd) const
{
    if (r_pd->packet_type == PACKET_TYPE_BINARY) {
        PbCommandDispatcher::bulkDispatch(this, r_pd->data);
    } else {
        Logger::log("3: warning packet type=%d, data size=%ld\n",
            r_pd->packet_type, r_pd->data.size());
        
        for (int i = 0; i < r_pd->data.size(); i++) {
            printf("%p ", r_pd->data[i]);
        }
        printf("\n");
    }
}

void UserClient::onReceiveFinish() const
{
    session->read();
}

void UserClient::onSendFinish() const
{
    //
}

void UserClient::onError(boost::system::error_code ec) const
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

long UserClient::getKey() const
{
    return reinterpret_cast<long>(session);
}
