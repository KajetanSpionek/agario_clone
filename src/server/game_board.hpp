/*!
*\file      game_board.hpp
*\author    Wojciech Przybysz, Kajetan Spionek
*
*/
#ifndef WEBSOCKET_GAME_BOARD_HPP
#define WEBSOCKET_GAME_BOARD_HPP

#include <string>
#include <algorithm>
#include <deque>
#include <set>
#include <map>
#include <utility>
#include <list>
#include <vector>
#include <iostream>
#include <cmath>
#include <atomic>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "dataframe.hpp"
#include "player.hpp"
#include "foodItem.hpp"
#include "ball.hpp"

namespace websocket {
  

    typedef std::deque<Dataframe> message_queue;

    class GameBoard
    {
    public:
        GameBoard(); 
        /// Join the game_board.
        void join(player_ptr participant);

        /// Leave the game_board.
        void leave(player_ptr participant);

        /// Deliver a chat message to all participant in the game_board.
        void deliver(const Dataframe& msg, player_ptr source);

        ///return map dimentions
        static double getMapX() { return mapX_;}
        static double getMapY() { return mapY_;}

    private:
        /// Deliver a message to all participant in the game_board.
        void deliver(const Dataframe& msg);

        /// Update a number of connected participants. 
        void updateParticipants();

        ///Methods of game logic
        void addNFoodItem(int n);

        ///create new foodItem 
        food_ptr getFood(double& x,double& y);

        ///erase foodItem from collection and informs other players
        void eraseFood(int id);

        //add new ball for new player and update other players
        void addNewBall(player_ptr participant,std::string nick);
        
        ///create new ball for player
        ball_ptr getBall(double& x ,double& y ,double radius);
        
        ///Sends balls and foods position to players
        void sendGameState(player_ptr participant);
       
        //delete ball and update players
        void eraseBall(player_ptr participant);

        //main movement processing
        void processMovement(const Dataframe& msg, player_ptr source);

        ///method of initial handshake - checks validity of inserted user nick
        void isNickValid(const Dataframe& msg,player_ptr source);

        ///on valid nick defines set of methods used to initiate the game
        void addPlayerToGame(const Dataframe& msg,player_ptr source);
 
        //collection of participants
        std::set<player_ptr> participants_;
        
        //limit of message_queue
        enum { max_recent_msgs = 1000 };

        //queue of messages to be delivered
        message_queue recent_msgs_;

        //Map of the balls parameters
        static const double mapX_;
        static const double mapY_;

        //parameters of balls ballot
        static const double foodItemMarigin_;
        static const double ballMarigin_;

        //foodItems const params
        static const int initialFood_ ; 
        static const int newPlayerFood_ ; 

        //how many parameters foodItem is categorized by
        static const int initialFoodParams_;
        static const int foodRadius_ ;

        static const int onEatenNewItems_;
        static const int initBallRadius_;

        //colletion of possitions occupied by other balls
        typedef std::set<std::pair<int,int>> occupied_pos;
        static occupied_pos occupiedPos_;

        //collection of balls        
        typedef std::map<player_ptr,ball_ptr > balls_container;
        static balls_container balls_;

        typedef std::map<int,element_ptr > elements_container;
        static elements_container elements_;

        //dataframe headers
        static const std::string messageOp_;
        static const std::string movementOp_;
        static const std::string nickCheckOp_;
        static const std::string newPlayerStatusOp_;
    
    };
    
} // namespace websocket

#endif // WEBSOCKET_GAME_BOARD_HPP