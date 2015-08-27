#ifndef WS_CLIENT_CONTROLLER_H
#define WS_CLIENT_CONTROLLER_H

#include "smart_ptr.hpp"
#include "network/websocket/client/websocket_async.h"

#include "common/data_buffer.h"
#include <list>
#include <functional>

typedef std::function<bool(bool, boost::asio::ssl::verify_context&)> VerifyCallback;

class WsClientController : public SmartPtr<WsClientController>
{
    friend class WsClientFactory;
    
public:
    enum class VerifyMode {
        VERIFY_PEER
    };
    
    typedef boost::intrusive_ptr<WsClientController> ptr;
    
    static WsClientController::ptr getInstance();
    void run();
    void stopController();
    
    void startWebsocket(client::WebsocketAsync* socket);
    void stopWebsocket(client::WebsocketAsync* socket);
    
    void setVerifyMode(VerifyMode verify_mode);
    void setCertificate(const std::string& filename);
    void setCertificate(DataBuffer::ptr data);
    void setVerifyCallback(VerifyCallback callback);
    
private:
    WsClientController();
    
    std::list<client::WebsocketAsync*> client_list;
    boost::asio::io_service ios;
    boost::asio::io_service::work work;
    boost::asio::ssl::context ssl_context;
    
    bool running;
    VerifyCallback verify_callback;
};

#endif
