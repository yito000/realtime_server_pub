#ifndef WS_CLIENT_CONTROLLER_H
#define WS_CLIENT_CONTROLLER_H

#include "smart_ptr.hpp"
#include "network/websocket/client/websocket_async.h"

#include <list>

class WsClientController : public SmartPtr<WsClientController>
{
    friend class WsClientFactory;
    
public:
    typedef boost::intrusive_ptr<WsClientController> ptr;
    
    static WsClientController::ptr getInstance();
    void run();
    void stopController();
    
    void startWebsocket(client::WebsocketAsync* socket);
    void stopWebsocket(client::WebsocketAsync* socket);
    
private:
    WsClientController();
    
    std::list<client::WebsocketAsync*> client_list;
    boost::asio::io_service ios;
    boost::asio::io_service::work work;
    boost::asio::ssl::context ssl_context;
    
    bool running;
};

#endif
