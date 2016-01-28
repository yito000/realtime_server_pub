#ifndef WS_CLIENT_CONTROLLER_H
#define WS_CLIENT_CONTROLLER_H

#include "fw_env.h"
#include "network/websocket/client/websocket_async.h"

#include "common/data_buffer.h"

#include <list>
#include <functional>
#include <boost/thread.hpp>
#include <atomic>

BEGIN_NS

typedef std::function<bool(bool, boost::asio::ssl::verify_context&)> VerifyCallback;

class WsClientController : public SmartPtr<WsClientController>
{
    friend class WsClientFactory;
    
public:
    enum class VerifyMode {
        VERIFY_NONE,
        VERIFY_PEER,
        VERIFY_FAIL_IF_NO_PEER_CERT
    };
    
    typedef boost::intrusive_ptr<WsClientController> ptr;
    
    static WsClientController::ptr getInstance();
    void run();
    void stopController();
    
    void pause();
    void resume();
    
    void startWebsocket(client::WebsocketAsync* socket, 
        client::HandShakeRequest::ptr h_req);
    void stopWebsocket(client::WebsocketAsync* socket);
    
    void setVerifyMode(VerifyMode verify_mode);
    void setCertificate(const std::string& filename);
    void setCertificate(DataBuffer::ptr data);
    
    void setVerifyCallback(VerifyCallback callback);
    
private:
    enum {
        SERVER_RUNNING,
        SERVER_PAUSED,
        SERVER_RESUME,
        SERVER_END
    };
    
    WsClientController();
    
    std::list<client::WebsocketAsync*> client_list;
    boost::asio::io_service ios;
    boost::asio::io_service::work work;
    boost::asio::ssl::context ssl_context;
    
    boost::condition_variable ws_cond;
    boost::mutex ws_mutex;
    
    std::atomic<int> server_flag;
    VerifyCallback verify_callback;
};

END_NS

#endif
