/*!
*\file      main.cpp
*\author    
*\date      
*/
#include <iostream>
#include "webserver.hpp"
#include "element.hpp"
#include "player.hpp"
#include "gameBoard.hpp"

int main(int argc, char* argv[]){

    std::cout << "In main: " << std::endl;
    WebServer s;
    s.init();
    return 0;
}