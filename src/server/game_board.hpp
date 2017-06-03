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

    /// The chat game_board class.
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
        void addNewBall(player_ptr participant);
        
        ///create new ball for player
        ball_ptr getBall(double& x ,double& y ,double& radius);
        
        ///Sends balls and foods position to players
        void sendGameState(player_ptr participant);
       
        //delete ball and update players
        void eraseBall(player_ptr participant);
        //void eraseElement(player_ptr participant); //erasing  food_item
        //void eraseElement(int id); //erasing food

        //main movement processing
        void processMovement(const Dataframe& msg, player_ptr source);

        void isNickValid(const Dataframe& msg,player_ptr source);

        void addPlayerToGame(const Dataframe& msg,player_ptr source);

        ///when player is eaten send statistics of the game
        void sendStats(player_ptr participant);
 
        std::set<player_ptr> participants_;
        //limit of message_queue
        enum { max_recent_msgs = 1000 };
        message_queue recent_msgs_;

        //Map of the balls
        static const double mapX_;
        static const double mapY_;
        static const double foodItemMarigin_;
        static const double ballMarigin_;

        //foodItems const params
        static const int initialFood_ ; //50
        static const int newPlayerFood_ ; //5
        static const int initialFoodParams_;
        const int foodRadius_ = 3;

        typedef std::set<std::pair<int,int>> occupied_pos;
        static occupied_pos occupiedPos_;

        //const int onEatenNewItems_ = 1;
        //const int initBallRadius_ = 20;

        //static const int foodRadius_{3};

        ///map containing state of gameplay with objects IDs
        //int IdCount_;
        //std::atomic<int> IdCount_;
        //typedef std::vector<std::vector<int> > game_map;
        //static game_map IdMap_;

        
        typedef std::map<player_ptr,ball_ptr > balls_container;
        static balls_container balls_;

        typedef std::map<int,element_ptr > elements_container;
        //typedef std::map<std::atomic<int> ,element_ptr > elements_container;
        static elements_container elements_;

        typedef std::map<int,player_ptr > id_to_player;
        static id_to_player idToPlayer_;
    
    };

    
} // namespace websocket

#endif // WEBSOCKET_GAME_BOARD_HPP