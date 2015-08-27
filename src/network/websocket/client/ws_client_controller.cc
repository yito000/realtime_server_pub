#include "ws_client_controller.h"

#include "common/file/file_util.h"

#include "atomic/atomic_operator.hpp"
#include <boost/thread.hpp>

WsClientController::ptr WsClientController::getInstance()
{
    static WsClientController::ptr inst = new WsClientController;
    
    return inst;
}

void WsClientController::run()
{
    AtomicOperator<bool>::lock_test_and_set(&running, true);
    
    while (running) {
        ios.poll();
        
        boost::this_thread::sleep_for(boost::chrono::milliseconds(16));
    }
}

void WsClientController::stopController()
{
    AtomicOperator<bool>::lock_test_and_set(&running, false);
}

void WsClientController::startWebsocket(client::WebsocketAsync* socket)
{
    ios.post([this, socket]() {
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
    ios.post([this, verify_mode]() {
            switch (verify_mode) {
                case VerifyMode::VERIFY_PEER:
                    ssl_context.set_verify_mode(
                        boost::asio::ssl::context::verify_peer);
                    break;
                
                default:
                    break;
            }
        });
}

void WsClientController::setCertificate(const std::string& filename)
{
    std::string a_filename = filename;
    ios.post([this, a_filename]() {
            auto data = FileUtil::getInstance()->getFileStream(a_filename)->readAll();
            
            std::vector<char> verify_file;
            verify_file.reserve(data->getSize());
            
            for (int i = 0; i < data->getSize(); i++) {
                verify_file.push_back(data->getBuffer()[i]);
            }
            
            ssl_context.add_certificate_authority(
                boost::asio::buffer(verify_file));
        });
}

void WsClientController::setCertificate(DataBuffer::ptr data)
{
    ios.post([this, data]() {
            std::vector<char> verify_file;
            verify_file.reserve(data->getSize());
            
            for (int i = 0; i < data->getSize(); i++) {
                verify_file.push_back(data->getBuffer()[i]);
            }
            
            ssl_context.add_certificate_authority(
                boost::asio::buffer(verify_file));
        });
}

void WsClientController::setVerifyCallback(VerifyCallback callback)
{
    ios.post([this, callback]() {
            ssl_context.set_verify_callback(callback);
        });
}

// private member function
WsClientController::WsClientController() :
    work(ios),
    ssl_context(ios, boost::asio::ssl::context::tlsv1),
    running(false)
{
    //
}
