#include "game_board.hpp"

namespace websocket {

    //std::vector<std::vector<int> > ballMap_;
    GameBoard::foods_container GameBoard::foods_;
    GameBoard::balls_container GameBoard::balls_;
    
    
    GameBoard::GameBoard()
    {
       // ballMap_.resize( mapY_ ,vector<int>(mapX_,0));
    }

    void GameBoard::join(player_ptr participant)
    {
        participants_.insert(participant);

        updateParticipants();

        std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
            boost::bind(&Player::deliver, participant, _1));

        addNewBall(participant);    

        addInitialFoodItem();
        
        sendGameState();
 
    }

    void GameBoard::leave(player_ptr participant)
    {
        participants_.erase(participant);

        updateParticipants();
    }

    void GameBoard::deliver(const Dataframe& msg, player_ptr source)
    {
        
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
        int x_temp;
        int y_temp;

        boost::random::mt19937 gen(static_cast<int>(std::time(0)));

        for( int i = 0; i < initialFoodParams_*initialFood_ ; i++)
        {
            //CHANGE TO DO _ WHILE LOOP
           // while( checkIfFreeSpace() ) 
           // {
                boost::random::uniform_int_distribution<> distX(foodItemMarigin_, mapX_ - foodItemMarigin_);
                x_temp = distX(gen);
                
                boost::random::uniform_int_distribution<> distY(foodItemMarigin_, mapY_ - foodItemMarigin_);
                y_temp = distY(gen);
         //   }
            
            foods_.push_back(getFood(x_temp,y_temp));
            //ballMap_[x_temp,y_temp] = 1;
            
        }
        
    }

    std::shared_ptr<FoodItem> GameBoard::getFood(int& x,int& y)
    {
        return std::make_shared<FoodItem>(x,y);
    }

    void GameBoard::addNewBall(player_ptr participant)
    {
        int initBallRadius_ = 20;
        int x_temp;
        int y_temp;

        boost::random::mt19937 gen(static_cast<int>(std::time(0)));

        //CHANGE TO DO _ WHILE LOOP
       // while( checkIfFreeSpace() ) 
       // {
            boost::random::uniform_int_distribution<> distX(ballMarigin_, mapX_ - ballMarigin_);
            x_temp = distX(gen);
            
            boost::random::uniform_int_distribution<> distY(ballMarigin_, mapY_ - ballMarigin_);
            y_temp = distY(gen);
       // }
        
        balls_.insert(std::make_pair(participant, getBall(x_temp,y_temp,initBallRadius_)));
        //ballMap_[x_temp,y_temp] = 1;
            
        
    }

    std::shared_ptr<Ball> GameBoard::getBall(int& x,int& y,int& radius)
    {
        return std::make_shared<Ball>(x,y,radius);
    }

    void GameBoard::sendGameState()
    {
        //send balls
        std::string header_balls = "newBall:";

        balls_container::iterator j;

        for(j = balls_.begin(); j != balls_.end(); j++ )
        {
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getX());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getY());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getRadius());
        }

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_balls)));

        //send foods

        foods_container::iterator i;
        
        std::string header_foods = "newFood:";

        for( i = foods_.begin(); i != foods_.end(); i++)
        {
       
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((*i)->getX());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((*i)->getY());
        }

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_foods)));

    }

    bool GameBoard::checkIfFreeSpace()
    {

        return false;
    }

} // namespace websocket