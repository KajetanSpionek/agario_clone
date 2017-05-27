#include "game_board.hpp"
#include <mutex>

namespace websocket {

    GameBoard::elements_container GameBoard::elements_;
    GameBoard::balls_container GameBoard::balls_;
    GameBoard::game_map GameBoard::IdMap_;
    GameBoard::id_to_player GameBoard::idToPlayer_;
    
    GameBoard::GameBoard()
    {
        IdMap_.resize( mapY_ ,std::vector<int>(mapX_,0));
        //set initial food to game board
        addNFoodItem(initialFood_);
    }

    void GameBoard::join(player_ptr participant)
    {
        //send current game state to new player
        sendGameState(participant);

        //add new ball and participant and send to everyone
        addNewBall(participant);    

        //add N new food and send to everyone
        addNFoodItem(newPlayerFood_);
        
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
        
        for( const auto& i : temp )
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
            processMovement(msg, source);
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
        int y_temp;
        int id;
        elements_container tmp_foods;
        elements_container::iterator it;

        int dummy_iter = 1;
        
        boost::random::mt19937 gen(static_cast<int>(std::time(0)));
        //generator loop to loose its entropy
        for(int k = 0; k < dummy_iter; ++k)
        {
            boost::random::uniform_int_distribution<> dummy(foodItemMarigin_, mapX_ - foodItemMarigin_);
            dummy(gen);
        }


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
            while( IdMap_.at(x_temp).at(y_temp) != 0 ) ; // while position is not empty
            
            auto temp_food = getFood(x_temp,y_temp);

            id = temp_food->getId();

            auto new_food = elements_.insert(std::make_pair(id,temp_food));
            
            tmp_foods.insert((*new_food.first));

            IdMap_.at(x_temp).at(y_temp) = id; 

        }

        //food update message loop

        std::string header_foods = "newFood:";

        for( it = tmp_foods.begin(); it != tmp_foods.end(); it++)
        {
            
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getId());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getX());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getY());

            std::cout << ((it->second)->getId()) << std::endl;
        }

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        //send to all participants

        deliver(frm_foods);

    }

    food_ptr GameBoard::getFood(int& x,int& y)
    {
        return std::make_shared<FoodItem>(x,y, foodRadius_ );
    }

    
    void GameBoard::eraseFood(int id)
    {
        element_ptr food;
        food = elements_.at(id);
        std::cout << "deleting food" << std::endl;
        IdMap_.at(food->getX()).at(food->getY()) = 0;


        std::string header_foods = "deleteFood:";

        header_foods = header_foods + " " + boost::lexical_cast<std::string>(id);
        //header_foods = header_foods + " " + boost::lexical_cast<std::string>(food->getX());
        //header_foods = header_foods + " " + boost::lexical_cast<std::string>(food->getY());

        //erase from collection
        elements_.erase(id);

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        deliver(frm_foods);
        /*
        std::for_each(participants_.begin(), participants_.end(),
            boost::bind(&Player::deliver, _1, boost::ref(frm_foods)));
        */
    }


    void GameBoard::addNewBall(player_ptr participant)
    {
        int initBallRadius_ = 20;
        int x_temp;
        int y_temp;
        int id;
        
        int dummy_iter = 1;
        
        boost::random::mt19937 gen(static_cast<int>(std::time(0)));
        //generator loop to loose its entropy
        for(int k = 0; k < dummy_iter; ++k)
        {
            boost::random::uniform_int_distribution<> dummy(ballMarigin_, mapX_ - ballMarigin_);
            dummy(gen);
        }

        do
        {
                boost::random::uniform_int_distribution<> distX(ballMarigin_, mapX_ - ballMarigin_);
                x_temp = distX(gen);
                
                boost::random::uniform_int_distribution<> distY(ballMarigin_, mapY_ - ballMarigin_);
                y_temp = distY(gen);
        } 
        while( IdMap_.at(x_temp).at(y_temp) != 0 ) ; // while position is not empty
            
        auto new_ball = balls_.insert(std::make_pair(participant, getBall(x_temp,y_temp,initBallRadius_)));

        id = ((new_ball.first)->second)->getId();
        elements_.insert(std::make_pair(id,(new_ball.first)->second )) ;       

        IdMap_.at(x_temp).at(y_temp) = (id); 
        
        idToPlayer_.insert( std::make_pair( id, participant ) );

        std::cout << "In addNewBall(): " << std::endl;
        std::cout << id << std::endl;

        //send new ball to new player

        std::string header = "newPlayerBall:";

        header = header + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getId());
        header = header + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getX());
        header = header + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getY());
        header = header + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getRadius());
        header = header + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getColor());

        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        participant->deliver(frm);

        //send new ball to enemies

        std::string header_balls = "newBall:";

        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getId());
        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getX());
        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getY());
        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getRadius());
        header_balls = header_balls + " " + boost::lexical_cast<std::string>(((new_ball.first)->second)->getColor());
        

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        deliver(frm_balls);
        //send enemies

        //insert new participant 
        participants_.insert(participant);

        updateParticipants();
             
    }

    ball_ptr GameBoard::getBall(int& x,int& y,int& radius)
    {
        return std::make_shared<Ball>(x,y,radius);
    }

    void GameBoard::eraseBall(player_ptr participant)
    {
        
        ball_ptr ball;
        ball = balls_.at(participant);

        int id = ball->getId();
        IdMap_.at(ball->getX()).at(ball->getY()) = 0;

        std::string header_balls = "deleteBall:";

        header_balls = header_balls + " " + boost::lexical_cast<std::string>(id);
        
        elements_.erase(id);
        
        //erase from balls collection
        balls_.erase(participant);

        std::cout << "header" << std::endl;

        //send looser end of game frame, delete him from collection
        std::string header = "endOfGame:";

        header = header + " " + boost::lexical_cast<std::string>(ball->getFoodNum());
        header = header + " " + boost::lexical_cast<std::string>(ball->getBallNum());
        header = header + " " + boost::lexical_cast<std::string>(ball->getRadius());

        std::cout << header << std::endl;
        
        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        participants_.erase(participant);
        participant->deliver(frm);

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        deliver(frm_balls);
        

    }

    void GameBoard::sendGameState(player_ptr participant)
    {
        int id;
        //send balls
        std::string header_balls = "gameStateBall:";

        balls_container::iterator j;

        for(j = balls_.begin(); j != balls_.end(); j++ )
        {
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getId());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getX());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getY());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getRadius());
            header_balls = header_balls + " " + boost::lexical_cast<std::string>((j->second)->getColor());   
        }

        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        participant->deliver(frm_balls);
        
        //send foods
        
        elements_container::iterator i;
        
        std::string header_foods = "gameStateFood:";

        for( i = elements_.begin(); i != elements_.end(); i++)
        {
            id = (i->second)->getId();
            if ( !(idToPlayer_.count(id) > 0) )
            {
                header_foods = header_foods + " " + boost::lexical_cast<std::string>(id);
                header_foods = header_foods + " " + boost::lexical_cast<std::string>((i->second)->getX());
                header_foods = header_foods + " " + boost::lexical_cast<std::string>((i->second)->getY());
            }
        }

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        participant->deliver(frm_foods);
        
    }

    
   

    void GameBoard::processMovement(const Dataframe& msg, player_ptr source)
     {
        int onEatenNewItems_ = 1;
        std::vector<boost::uint8_t> temp;
        std::string rxss;
        std::string ryss;
        std::string temps;
        const char delimit_ = ':';
        const uint8_t delim = static_cast<uint8_t>(delimit_);
        const char delimitArg_ = ',';
        
        int radius;
        int id_source;
        int rx;
        int ry;

        int offset_x;
        int offset_y;
        int dist;
        int nx;
        int ny;
        int nradius;

        ball_ptr ball_source = balls_.at(source);

        radius = ball_source->getRadius();
        
        id_source = ball_source->getId();
        

        auto it_beg = std::find(msg.payload.begin(), msg.payload.end(),delim);
        std::copy(++it_beg, msg.payload.end(), std::back_inserter(temp));

        for(const auto& i : temp)
        {
            temps = temps +  boost::lexical_cast<std::string>(i);
        }

        int n = temps.find(delimitArg_);
        rxss = temps.substr(0,n);
        ryss = temps.substr(n+1,temps.size());

        rx = boost::lexical_cast<int>(rxss);
        ry = boost::lexical_cast<int>(ryss);

        
        IdMap_.at(ball_source->getX()).at(ball_source->getY()) = 0;
        
        ball_source->setX(rx);
        ball_source->setY(ry);

        std::vector<int> neighbourId;
        int id;

        if( (id = IdMap_.at(rx).at(ry) ) != 0 )
        {
            neighbourId.push_back(id);
        }


        IdMap_.at(rx).at(ry) = ball_source->getId();
        //boundaries check
        
        //default offset
        offset_y = radius;
        offset_x = radius;
        
        if( ( rx - radius) <= 0)
            offset_x = 0;
        else if ( rx + radius >= mapX_ - 1) 
            offset_x = 0;
        else if ( ry - radius <= 0)
            offset_y = 0;
        else if ( ry + radius >= mapY_ - 1)
            offset_y = 0; 
        else

        
        for( int i = rx - offset_x ; i < rx + offset_x; i++)
        {
            for(int j = ry - offset_y; j < ry + offset_y; j++)
            {
                id = IdMap_.at(i).at(j);
                if( id != 0 )
                {
                    if( id != id_source )
                    {
                        neighbourId.push_back(id);
                        break; //delete one element at a time
                    }
                    else
                    {
                        auto it = elements_[i];
                    }
                }
            }
        }


        if(!neighbourId.empty())
        {
            for(const auto& i : neighbourId )
            {
                auto it = elements_.at(i);
                nx = it->getX();
                ny = it->getY();
                nradius = it->getRadius();
                //calculate distance
                
                dist = std::sqrt( (rx-nx)*(rx-nx) + (ry-ny)*(ry-ny));
                if(dist < radius)
                {
                    if(nradius == foodRadius_) //change to const
                    {
                        std::cout << "eraseFood invoke" << std::endl;
                        eraseFood(i);
                        addNFoodItem(onEatenNewItems_);    //change to const
                        //std::cout << i << std::endl;
                        ball_source->setRadius(radius + nradius);
                        //std::cout << ball_source->getRadius() << std::endl;
                        ball_source->incFood();

                    }
                    else
                    {
                       if( radius > nradius)
                       {
                           player_ptr owner = idToPlayer_[i];
                           eraseBall(owner);
                           ball_source->setRadius(radius + nradius);
                           //std::cout << ball_source->getRadius() << std::endl;
                           ball_source->incBall();
                       }
                       else 
                       {

                       } 

                    }
                                        
                }       
               
            }
        }


        std::string header = "ballUpdate:";
        
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getId());
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getX());
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getY());
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getRadius());
        
        Dataframe frm;
        
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        deliver(frm);

     }

} // namespace websocket