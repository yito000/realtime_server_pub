#include "ws_client_controller.h"

#include "common/file/file_util.h"

#include <boost/thread.hpp>

BEGIN_NS

WsClientController::ptr WsClientController::getInstance()
{
    static WsClientController::ptr inst = new WsClientController;
    
    return inst;
}

void WsClientController::run()
{
    while (server_flag != SERVER_END) {
        if (server_flag == SERVER_PAUSED) {
            boost::mutex::scoped_lock lock(ws_mutex);
            ws_cond.wait(lock);
            
            continue;
        } else if (server_flag == SERVER_RESUME) {
            server_flag.store(SERVER_RUNNING);
            continue;
        }
        
        ios.poll();
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(16));
    }
}

void WsClientController::stopController()
{
    server_flag.store(SERVER_END);
    
    boost::mutex::scoped_lock lock(ws_mutex);
    ws_cond.notify_all();
}

void WsClientController::pause()
{
    server_flag.store(SERVER_PAUSED);
}

void WsClientController::resume()
{
    server_flag.store(SERVER_RESUME);
    
    boost::mutex::scoped_lock lock(ws_mutex);
    ws_cond.notify_all();
}

void WsClientController::startWebsocket(client::WebsocketAsync* socket,
    client::HandShakeRequest::ptr h_req)
{
    ios.post([this, socket, h_req]() {
            socket->connect(h_req);
            client_list.push_back(socket);
        });
}

void WsClientController::stopWebsocket(client::WebsocketAsync* socket)
{
    ios.post([this, socket]() {
            auto it = client_list.begin();
            
            for (; it != client_list.end(); ++it) {
                if (socket == *it) {
                    socket->destroyAsync();
                    client_list.erase(it);
                    
                    break;
                }
            }
        });
}

void WsClientController::setVerifyMode(VerifyMode verify_mode)
{
    auto func = [this, verify_mode]() {
        switch (verify_mode) {
            case VerifyMode::VERIFY_NONE:
                ssl_context.set_verify_mode(
                    boost::asio::ssl::context::verify_none);
                break;
            
            case VerifyMode::VERIFY_PEER:
                ssl_context.set_verify_mode(
                    boost::asio::ssl::context::verify_peer);
                break;
            
            case VerifyMode::VERIFY_FAIL_IF_NO_PEER_CERT:
                ssl_context.set_verify_mode(boost::asio::ssl::context::verify_peer |
                    boost::asio::ssl::context::verify_fail_if_no_peer_cert);
                break;
            
            default:
                break;
        }
    };
    
    func();
}

void WsClientController::setCertificate(const std::string& filename)
{
    std::string a_filename = filename;
    
    auto func = [this, a_filename]() {
        auto data = FileUtil::getInstance()->getFileStream(a_filename)->readAll();
        
        std::vector<char> verify_file;
        verify_file.reserve(data->getSize());
        
        for (int i = 0; i < data->getSize(); i++) {
            verify_file.push_back(data->getBuffer()[i]);
        }
        
        ssl_context.add_certificate_authority(
            boost::asio::buffer(verify_file));
    };
    
    func();
}

void WsClientController::setCertificate(DataBuffer::ptr data)
{
    auto func = [this, data]() {
        std::vector<char> verify_file;
        verify_file.reserve(data->getSize());
        
        for (int i = 0; i < data->getSize(); i++) {
            verify_file.push_back(data->getBuffer()[i]);
        }
        
        ssl_context.add_certificate_authority(
            boost::asio::buffer(verify_file));
    };
    
    func();
}

void WsClientController::setVerifyCallback(VerifyCallback callback)
{
    auto func = [this, callback]() {
        ssl_context.set_verify_callback(callback);
    };
    
    func();
}

// private member function
WsClientController::WsClientController() :
    work(ios),
    ssl_context(ios, boost::asio::ssl::context::tlsv12),
    server_flag(SERVER_RUNNING)
{
    //
}

END_NS
