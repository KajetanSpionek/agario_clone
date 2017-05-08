#include "game_board.hpp"

namespace websocket {
  

    void GameBoard::join(player_ptr participant)
    {
        participants_.insert(participant);

        updateParticipants();

        std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
            boost::bind(&Player::deliver, participant, _1));

        addInitialFoodItem();


    }

    void GameBoard::leave(player_ptr participant)
    {
        participants_.erase(participant);

        updateParticipants();
    }

    void GameBoard::deliver(const Dataframe& msg, player_ptr source)
    {
        for(auto i: msg.payload)
            std::cout << i;
        std::cout << std::endl;

        std::string header = "log:" + source->getId() + ": ";

        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));
        std::copy(msg.payload.begin(), msg.payload.end(), std::back_inserter(frm.payload));

        deliver(frm);
    }

    void GameBoard::deliver(const Dataframe& msg)
    {
        
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(msg)));
    }

    void GameBoard::updateParticipants()
    {
        std::string header = "connected:" + boost::lexical_cast<std::string>(participants_.size());

        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm)));
    }

    void GameBoard::addInitialFoodItem()
    {
        const unsigned int newItemEl = 10;
        std::cout << "addNewFoodItem" << std::endl;
        std::vector<unsigned int> tab(newItemEl,0);

        boost::random::mt19937 gen(static_cast<unsigned int>(std::time(0)));

        for( std::vector<unsigned int>::iterator it = tab.begin(); it < tab.end(); it++)
        {
            //check if elements overlay
            boost::random::uniform_int_distribution<> dist(1, 200);
            *it = dist(gen);
        }

        std::string header = "newFood:";
        for( auto i : tab)
        {
            header = header + " " + boost::lexical_cast<std::string>(i);
        }
        std::cout << header << std::endl;

        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm)));
    }

} // namespace websocket