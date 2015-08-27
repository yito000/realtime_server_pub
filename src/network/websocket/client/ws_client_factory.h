#ifndef WS_CLIENT_FACTORY_H
#define WS_CLIENT_FACTORY_H

#include "smart_ptr.hpp"

#include "network/websocket/client/websocket_async.h"

class WsClientController;

#define DEFAULT_WEBSOCKET_TIMEOUT_MILLIS (60 * 1000)

class WsClientFactory : public SmartPtr<WsClientFactory>
{
public:
    typedef boost::intrusive_ptr<WsClientFactory> ptr;
    
    static WsClientFactory::ptr getInstance();
    
    client::WebsocketAsync* newClient(const std::string& _host, 
        unsigned short _port, int _timeout_millis = DEFAULT_WEBSOCKET_TIMEOUT_MILLIS);
    client::WebsocketAsync* newSSLClient(const std::string& _host, 
        unsigned short _port, int _timeout_millis = DEFAULT_WEBSOCKET_TIMEOUT_MILLIS);
    
private:
    WsClientFactory(WsClientController& wsc_controller);
    
    boost::asio::io_service& ios;
    boost::asio::ssl::context& ssl_context;
};

#endif
