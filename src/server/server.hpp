#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <boost/asio.hpp>



class Server{
public:
    void init();
    void run();
    void stop();
private:
   // std::string const  host_ = "localhost";
    
};

#endif //SERVER_HPP