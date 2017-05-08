#ifndef WEBSOCKET_GAME_BOARD_HPP
#define WEBSOCKET_GAME_BOARD_HPP

#include <string>
#include <algorithm>
#include <deque>
#include <set>
#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "dataframe.hpp"
#include "player.hpp"

namespace websocket {
  

    typedef std::deque<Dataframe> message_queue;

    /// The chat game_board class.
    class GameBoard
    {
    public:
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
        void addInitialFoodItem();

        //void detectCollision();
        //

        std::set<player_ptr> participants_;
        enum { max_recent_msgs = 100 };
        message_queue recent_msgs_;

        //Map of the balls
        const int mapX_{200};
        const int mapY_{400};
        //std::std::vector<std::std::vector<int>> ballMap_();
    };

} // namespace websocket

#endif // WEBSOCKET_GAME_BOARD_HPP