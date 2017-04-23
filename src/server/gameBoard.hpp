/*!
*\file      GameBoard.hpp
*\author    
*\date      
*/
#ifndef FOODITEM_HPP
#define FOODITEM_HPP

#include "element.hpp"
#include "player.hpp"
#include "webserver.hpp"

#include <iostream>
#include <memory>
#include <vector>

//MUST BE SINGLETON
class GameBoard {
public:
    //different argument of the method
    //OBSERVER PATTERN
    /*
   void registerElement();
   void unregisterElement();
   void notifyElements();*/

   static void getInstance();

   void run();

   //Methods of game logic
   void detectCollision();
   void addNewPlayer();
   void deletePlayer();
   void addNewFood();
   void deleteFood();

   void createInitialFood();

   void update(); //updates all Players in the board with all positions of elements
private:
    GameBoard();
    GameBoard(const GameBoard &) = delete;

    typedef std::shared_ptr<Element> observer;
    typedef std::vector<observer> observers;
    observers observersCollection_;
    typedef std::shared_ptr<Player> player;
    typedef std::vector<player> players;
    players playersCollection_;

    //
    typedef std::unique_ptr<WebServer> serverPtr;
    serverPtr server_;

    //mapSize
    const unsigned int xMapSize{2000}; //just to shut the compiler
    const unsigned int yMapSize{2000};

};

#endif //_FOODITEM_H_