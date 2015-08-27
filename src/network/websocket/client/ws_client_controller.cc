#include "ws_client_controller.h"

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

// private member function
WsClientController::WsClientController() :
    work(ios),
    ssl_context(ios, boost::asio::ssl::context::tlsv1),
    running(false)
{
    //
}
