/*!
*\file      main.cpp
*\author    
*\date      
*/
#include <iostream>
#include "server.hpp"
#include "element.hpp"
#include "player.hpp"
#include "gameBoard.hpp"

int main(int argc, char* argv[]){

    std::cout << "In main: " << std::endl;
    Server s;
    s.init();
    return 0;
}