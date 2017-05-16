#ifndef WEBSOCKET_PLAYER_HPP
#define WEBSOCKET_PLAYER_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "dataframe.hpp"

namespace websocket {
    
    class Player
    {
    public:
        virtual ~Player() {}
        virtual void deliver(Dataframe msg) = 0;
        virtual std::string getId() = 0;
    };

    typedef boost::shared_ptr<Player> player_ptr;
            
} // namespace websocket

#endif // WEBSOCKET_APPLICATIONS_CHAT_PARTICIPANT_HPP