#include "webserver.hpp"



//using boost::asio
void WebServer::init(){
    std::cout << "in init" << std::endl;

//adress_type 
    auto host = "127.0.0.1";
    boost::asio::io_service ios;
    boost::asio::ip::tcp::resolver r{ios};
    socket_type sock{ios};
    connect(sock,r.resolve(boost::asio::ip::tcp::resolver::query{host,"3000"}));

//send message
    beast::websocket::stream<boost::asio::ip::tcp::socket&> ws{sock};
    ws.handshake(host,"/");
    ws.write(boost::asio::buffer("Hello, world!"));
//reveive messsage
    boost::asio::streambuf sb;
    beast::websocket::opcode op;
    
    ws.read(op,sb);

    //std::cout << boost::lexical_cast<std::string>(sb.data());
    std::cout << beast::to_string(sb.data()); //boost utility function

    ws.close(beast::websocket::close_code::normal);
}