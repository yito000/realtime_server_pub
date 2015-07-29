#ifndef APP_SESSION_DELEGATE_H
#define APP_SESSION_DELEGATE_H

#include "network/websocket/server/session_delegate.h"
#include "thread/bidirectional_communicator.h"

class AppSessionDelegate : public server::SessionDelegate
{
public:
    AppSessionDelegate(BidirectionalCommunicator::ptr t_comm);

    virtual void onStart(server::WebsocketSession* session);
    virtual void onReceive(server::WebsocketSession* session,
        PacketData::ptr r_pd);
    virtual void onReceiveFinish(server::WebsocketSession* session);
    virtual void onSendFinish(server::WebsocketSession* session);
    virtual void onError(server::WebsocketSession* session,
        boost::system::error_code ec);

private:
    BidirectionalCommunicator::ptr task_comm;
};

#endif

