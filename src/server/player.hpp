/*!
*\file      Player.hpp
*\author    
*\date      
*/
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "stat.hpp"
#include "ball.hpp"

class Player{
public:
    Player();
    ~Player();
    Player(const Player &) = delete;
    unsigned int getId();
private:
    unsigned int id_;
    typedef std::shared_ptr<Ball> ballPtr;
    ballPtr ball_;
    Stat stats_;
};

#endif 
//_PLAYER_H_