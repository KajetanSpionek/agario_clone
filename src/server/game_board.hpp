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

    private:
        /// Deliver a message to all participant in the game_board.
        void deliver(const Dataframe& msg);

        /// Update a number of connected participants. 
        void updateParticipants();

        ///Methods of game logic
        void addNFoodItem(int n);

        food_ptr getFood(int& x,int& y);

        void eraseFood(int id);

        //Player ball
        void addNewBall(player_ptr participant);
        
        ball_ptr getBall(int& x ,int& y ,int& radius);
        
        ///Sends balls and foods position to players
        void sendGameState(player_ptr participant);
       
        void eraseBall(player_ptr participant);

        std::set<player_ptr> participants_;
        //limit of message_queue
        enum { max_recent_msgs = 100 };
        message_queue recent_msgs_;

        //Map of the balls
        static const int mapX_{200};
        static const int mapY_{400};
        static const int foodItemMarigin_{5};
        static const int ballMarigin_{10};

        static const int initialFood_ {10};
        static const int initialFoodParams_{2};

        int IdCount_;
        typedef std::vector<std::vector<int> > game_map;
        static game_map IdMap_;

        
        typedef std::map<player_ptr,ball_ptr > balls_container;
        static balls_container balls_;
        typedef std::map<int,food_ptr > foods_container;
        static foods_container foods_;

        //main movement processing
        void processMovement(player_ptr source);
    };

    
} // namespace websocket

#endif // WEBSOCKET_GAME_BOARD_HPP