#include "ws_client_factory.h"

#include "ws_client_controller.h"

WsClientFactory::ptr WsClientFactory::getInstance()
{
    static WsClientFactory::ptr inst = new WsClientFactory(*WsClientController::getInstance());
    
    return inst;
}

client::WebsocketAsync* WsClientFactory::newClient(const std::string& _host, 
    unsigned short _port, int _timeout_millis)
{
    auto ret = client::WebsocketAsync::create(ios, _host, _port, _timeout_millis);
    
    return ret;
}

client::WebsocketAsync* WsClientFactory::newSSLClient(const std::string& _host, 
    unsigned short _port, int _timeout_millis)
{
    auto ret = client::WebsocketAsync::createSSL(ios, ssl_context, _host, _port, _timeout_millis);
    
    return ret;
}

//
WsClientFactory::WsClientFactory(WsClientController& wsc_controller) :
    ios(wsc_controller.ios),
    ssl_context(wsc_controller.ssl_context)
{
    //
}
