/*!
*\file      WebsocketsServer.hpp
*\author    
*\date      
*/
#ifndef _WEBSOCKETSERVER_H_
#define _WEBSOCKETSERVER_H_
/*
#include <string>
#include "websocketapp/server.hpp"
#include "websocketapp/config/asio_no_tls.hpp"
//facade to network functions
//SINGLETON ???
//done to be compatible with Websocket++ library (first phase of test)
class WebsocketServer {
public:
//Not sure yet about return types
    static bool init();
    static void run();
    static void stop();

    static bool sendClose(std::string id);
    static bool sendData(std::string id, std::string data);
private:
    static bool getWebsocket(const std::string &id, websocketapp::connection_hdl &hdl);
    static websocketapp::server<websocketapp::config::asio> server;
    static pthread_rwlock_t websocketsLock;
}
*/
#endif //_WEBSOCKETSERVER_H_