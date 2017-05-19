#include "game_board.hpp"

namespace websocket {

    GameBoard::foods_container GameBoard::foods_;
    GameBoard::balls_container GameBoard::balls_;
    GameBoard::game_map GameBoard::IdMap_;
    
    
    GameBoard::GameBoard()
    {
        IdMap_.resize( mapY_ ,std::vector<int>(mapX_,0));
    }

    void GameBoard::join(player_ptr participant)
    {
        participants_.insert(participant);

        updateParticipants();

        std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
            boost::bind(&Player::deliver, participant, _1));

        //send current game state to new player
        sendGameState(participant);

        //add new ball and send to everyone
        addNewBall(participant);    

        //add N new food and send to everyone
        addNFoodItem(initialFood_);
        
        
 
    }

    void GameBoard::leave(player_ptr participant)
    {
        eraseBall(participant);

        participants_.erase(participant);

        updateParticipants();
    }

    void GameBoard::deliver(const Dataframe& msg, player_ptr source)
    {
        const std::string messageOp_ = "message";
        const std::string movementOp_ = "move";
        const char delimit_ = ':';

        std::string header = "log:" + source->getId() + ": ";

        std::vector<boost::uint8_t> temp;
        std::string temp_string;

        const uint8_t delim = static_cast<uint8_t>(delimit_);

        auto it_end = std::find(msg.payload.begin(), msg.payload.end(),delim);
        std::copy(msg.payload.begin(), it_end, std::back_inserter(temp));
        
        for( auto i : temp )
        {
            temp_string = temp_string + boost::lexical_cast<std::string>(i);
        }

        if( temp_string == messageOp_ )
        {
            Dataframe frm;
            std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));
            std::copy(msg.payload.begin(), msg.payload.end(), std::back_inserter(frm.payload));

            deliver(frm);
        }
        else if( temp_string == movementOp_)
        {
            processMovement(source);
        }
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

    void GameBoard::addNFoodItem(int n)
    {
        int x_temp;
        int y_temp;void deleteBall(player_ptr participant);
        foods_container tmp_foods;
        foods_container::iterator it;
        
        boost::random::mt19937 gen(static_cast<int>(std::time(0)));

        //food creation loop

        for( int i = 0; i < initialFoodParams_*n ; i++)
        {
            do
            {
                    boost::random::uniform_int_distribution<> distX(foodItemMarigin_, mapX_ - foodItemMarigin_);
                    x_temp = distX(gen);
                    
                    boost::random::uniform_int_distribution<> distY(foodItemMarigin_, mapY_ - foodItemMarigin_);
                    y_temp = distY(gen);
            } 
            while( IdMap_.at(y_temp).at(x_temp) != 0 ) ; // while position is not empty
            
            auto new_food = foods_.insert(std::make_pair(IdCount_,getFood(x_temp,y_temp)));

            tmp_foods.insert((*new_food.first));

            IdMap_.at(y_temp).at(x_temp) = (((new_food.first)->second)->getId()); 

        }

        //food update message loop

        std::string header_foods = "newFood:";

        for( it = foods_.begin(); it != foods_.end(); it++)
        {
       
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getX());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getY());
        }

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        //send to all participants

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_foods)));
        
        
    }

    food_ptr GameBoard::getFood(int& x,int& y)
    {
        return std::make_shared<FoodItem>(x,y,++IdCount_);
    }

    void GameBoard::eraseFood(int id)
    {
        food_ptr food;
        food = foods_[id];
        IdMap_.at(food->getY()).at(food->getX()) = 0;


        std::string header_foods = "deleteFood:";

        header_foods = header_foods + " " + boost::lexical_cast<std::string>(id);
        //header_foods = header_foods + " " + boost::lexical_cast<std::string>(food->getX());
        //header_foods = header_foods + " " + boost::lexical_cast<std::string>(food->getY());

        //erase from collection
        foods_.erase(id);

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_foods)));

    }

    void GameBoard::addNewBall(player_ptr participant)
    {
        int initBallRadius_ = 20;
        int x_temp;
        int y_temp;

        boost::random::mt19937 gen(static_cast<int>(std::time(0)));

        do
        {
                boost::random::uniform_int_distribution<> distX(ballMarigin_, mapX_ - ballMarigin_);
                x_temp = distX(gen);
                
                boost::random::uniform_int_distribution<> distY(ballMarigin_, mapY_ - ballMarigin_);
                y_temp = distY(gen);
        } 
        while( IdMap_.at(y_temp).at(x_temp) != 0 ) ; // while position is not empty
            
        auto new_ball = balls_.insert(std::make_pair(participant, getBall(x_temp,y_temp,initBallRadius_)));

        IdMap_.at(y_temp).at(x_temp) = (((new_ball.first)->second)->getId()); 

        std::string header_balls = "newBall:";


        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getX());
        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getY());
        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getRadius());
        

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        //send to all players
        
        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_balls)));
        
            
        
    }

    ball_ptr GameBoard::getBall(int& x,int& y,int& radius)
    {
        return std::make_shared<Ball>(x,y,radius,++IdCount_);
    }

    void GameBoard::eraseBall(player_ptr participant)
    {
        ball_ptr ball;
        ball = balls_[participant];
        IdMap_.at(ball->getY()).at(ball->getX()) = 0;

        std::string header_balls = "deleteBall:";

        header_balls = header_balls + " " + boost::lexical_cast<std::string>(ball->getId());
        //header_balls = header_balls + " " + boost::lexical_cast<std::string>(ball->getX());
        //header_balls = header_balls + " " + boost::lexical_cast<std::string>(ball->getY());

        //erase from balls collection
        balls_.erase(participant);

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_balls)));

        
    }

    void GameBoard::sendGameState(player_ptr participant)
    {
        //send balls
        std::string header_balls = "gameStateBall:";

        balls_container::iterator j;

        for(j = balls_.begin(); j != balls_.end(); j++ )
        {
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getX());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getY());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getRadius());
        }

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        deliver(frm_balls,participant);
        /*
        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_balls)));
        */

        //send foods

        foods_container::iterator i;
        
        std::string header_foods = "gameStateFood:";

        for( i = foods_.begin(); i != foods_.end(); i++)
        {
       
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((i->second)->getX());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((i->second)->getY());
        }

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        /*
        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_foods)));
        */
        deliver(frm_foods,participant);

    }

    void GameBoard::processMovement(player_ptr source)
     {

     }

} // namespace websocket