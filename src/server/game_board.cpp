#include "game_board.hpp"

namespace websocket {

    //static members initialization
    const int GameBoard::mapX_{3000};
    const int GameBoard::mapY_{3000};
    const double GameBoard::foodItemMarigin_{5};
    const double GameBoard::ballMarigin_{10};

    //foodItems const params
    const int GameBoard::initialFood_ {200}; 
    const int GameBoard::newPlayerFood_ {5}; 
    const int GameBoard::initialFoodParams_{1};
    const int GameBoard::foodRadius_{3};
    const int GameBoard::onEatenNewItems_{1};
    const int GameBoard::initBallRadius_{20};

    GameBoard::elements_container GameBoard::elements_;
    GameBoard::balls_container GameBoard::balls_;
    GameBoard::occupied_pos GameBoard::occupiedPos_;

    const std::string GameBoard::messageOp_ = "message";
    const std::string GameBoard::movementOp_ = "move";
    const std::string GameBoard::nickCheckOp_ = "newPlayerName";
    const std::string GameBoard::newPlayerStatusOp_ = "newPlayerStatus";

    GameBoard::GameBoard()
    {
        //set initial food to game board
        addNFoodItem(initialFood_);
    }

    void GameBoard::join(player_ptr participant)
    {

        /*
        New player joins the game
        */
    }

    void GameBoard::isNickValid(const Dataframe& msg,player_ptr source)
    {
        std::string nick;
        bool nick_occupied = false;


        const char delimit_ = ':';
        const uint8_t delim = static_cast<uint8_t>(delimit_);
        const std::string ok = "OK";
        const std::string taken = "TAKEN";

        std::vector<boost::uint8_t> temp;
        std::string temps;

        //parse incoming frame
        auto it_beg = std::find(msg.payload.begin(), msg.payload.end(),delim);
        std::copy(++it_beg, msg.payload.end(), std::back_inserter(temp));

        for(const auto& i : temp)
        {
            temps = temps +  boost::lexical_cast<std::string>(i);
        }

        nick = temps.substr(0,temps.size());
        
        std::cout << "New nick " << nick << std::endl;

        
        for( const auto & i: balls_ )
        {
            if( i.second->getNick() == nick )
            {
                nick_occupied = true;
            }
        }
        
        
        std::string header = "newPlayerValidNick:";

        if(nick_occupied)
        {
            header = header + boost::lexical_cast<std::string>(taken);
            
        }
        else
        {
            header = header + boost::lexical_cast<std::string>(ok);
        }

        std::cout << header << std::endl;

        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        source->deliver(frm);
        

    }

    void GameBoard::addPlayerToGame(const Dataframe& msg,player_ptr source)
    {
        std::cout << "add player to the game" << std::endl;
        std::string nick;

        std::string rdy_flag;
        const char delimit_ = ':';
        const uint8_t delim = static_cast<uint8_t>(delimit_);

        std::vector<boost::uint8_t> temp;
        std::string temps;
        const char delimit_arg = ',';

        //parse incoming frame
        auto it_beg = std::find(msg.payload.begin(), msg.payload.end(),delim);
        std::copy(++it_beg, msg.payload.end(), std::back_inserter(temp));

        for(const auto& i : temp)
        {
            temps = temps +  boost::lexical_cast<std::string>(i);
        }

        int n = temps.find(delimit_arg);
        rdy_flag = temps.substr(0,n);
        nick = temps.substr(n+1,temps.size());

        if( rdy_flag == "nrdy")
            return;
        else if ( rdy_flag == "rdy" )
        {
            std::cout << rdy_flag << " " << nick << std::endl;

            //sends map dimensions
            sendMapSize(source);

            //send current game state to new player
            sendGameState(source);

            //add new ball and participant and send to everyone
            addNewBall(source,nick);    

            //add N new food and send to everyone
            addNFoodItem(newPlayerFood_);
        }

    }

    void GameBoard::sendMapSize(player_ptr source)
    {
        std::string header = "mapSize:";


        header = header + " " + boost::lexical_cast<std::string>(getMapX());
        header = header + " " + boost::lexical_cast<std::string>(getMapY());
    

        std::cout << header << std::endl;

        Dataframe frm;
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        source->deliver(frm);

        std::cout << "in delivery " << header << std::endl;
        

    }

    void GameBoard::leave(player_ptr participant)
    {
        //in case of shutting down browser
        if ( participants_.count(participant) > 0)
        {
            eraseBall(participant);

            participants_.erase(participant);

            updateParticipants();
        }
        else
        {
            //do nothing
        }
    }

    void GameBoard::deliver(const Dataframe& msg, player_ptr source)
    {
        const char delimit = ':';

        std::string header = "log:" + source->getId() + ": ";

        std::vector<boost::uint8_t> temp;
        std::string temp_string;

        const uint8_t delim = static_cast<uint8_t>(delimit);

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
        else if( temp_string == nickCheckOp_)
        {
            isNickValid(msg,source);
        }
        else if( temp_string == newPlayerStatusOp_)
        {
            addPlayerToGame(msg,source);

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
        double x_temp;
        double y_temp;
        int id;
        elements_container tmp_foods;
        elements_container::iterator it;

        int dummy_iter = 10;
        
        boost::random::mt19937 gen(static_cast<int>(std::time(0)));

        //generator loop to loose its entropy
        for(int k = 0; k < dummy_iter; ++k)
        {
            boost::random::uniform_int_distribution<> dummy(foodItemMarigin_, mapX_ - foodItemMarigin_);
            dummy(gen);
        }


        //food creation loop
        std::pair<int,int> temp_pos;

        for( int i = 0; i < initialFoodParams_*n ; i++)
        {
            do
            {
                    boost::random::uniform_int_distribution<> distX(foodItemMarigin_, mapX_ - foodItemMarigin_);
                    x_temp = distX(gen);
                    
                    boost::random::uniform_int_distribution<> distY(foodItemMarigin_, mapY_ - foodItemMarigin_);
                    y_temp = distY(gen);

                    temp_pos = std::make_pair(x_temp,y_temp);
            } 
            while( occupiedPos_.count(temp_pos) != 0 ) ; // while position is not empty
            
            occupiedPos_.insert(temp_pos);
 
            auto temp_food = getFood(x_temp,y_temp);

            id = temp_food->getId();

            auto new_food = elements_.insert(std::make_pair(id,temp_food));

            std::cout << "map elements_.size():  " << elements_.size() << std::endl;
            
            tmp_foods.insert((*new_food.first));

        }

        //food update message loop

        std::string header_foods = "newFood:";

        for( it = tmp_foods.begin(); it != tmp_foods.end(); it++)
        {
            
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getId());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getX());
            header_foods = header_foods + " " + boost::lexical_cast<std::string>((it->second)->getY());

            std::cout << "id of new food" << ((it->second)->getId()) << std::endl;
        }

        
        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        //send to all participants

        deliver(frm_foods);


    }

    food_ptr GameBoard::getFood(double& x,double& y)
    {
        return std::make_shared<FoodItem>(x,y, foodRadius_ );
    }

    
    void GameBoard::eraseFood(int id)
    {
        std::cout << "eraseFood" << std::endl;
        element_ptr food;
        food = elements_.at(id);
        std::cout << "deleting food" << std::endl;
        
        std::pair<int,int> temp_pos = std::make_pair(food->getX(),food->getY());
        occupiedPos_.erase(temp_pos);

        std::string header_foods = "deleteFood:";

        header_foods = header_foods + " " + boost::lexical_cast<std::string>(id);
 
        //erase from collection
        elements_.erase(id);

        Dataframe frm_foods;
        std::copy(header_foods.begin(), header_foods.end(), std::back_inserter(frm_foods.payload));

        deliver(frm_foods);

    }


    void GameBoard::addNewBall(player_ptr participant,std::string nick)
    {
        double x_temp;
        double y_temp;
        int id;
        
        int dummy_iter = 1;
        
        boost::random::mt19937 gen(static_cast<int>(std::time(0)));

        //generator loop to loose its entropy
        for(int k = 0; k < dummy_iter; ++k)
        {
            boost::random::uniform_int_distribution<> dummy(ballMarigin_, mapX_ - ballMarigin_);
            dummy(gen);
        }

        std::pair<int,int> temp_pos;

        do
        {
                boost::random::uniform_int_distribution<> distX(ballMarigin_, mapX_ - ballMarigin_);
                x_temp = distX(gen);
                
                boost::random::uniform_int_distribution<> distY(ballMarigin_, mapY_ - ballMarigin_);
                y_temp = distY(gen);

                temp_pos = std::make_pair(x_temp,y_temp);
        } 
        while( occupiedPos_.count(temp_pos) != 0 ) ; // while position is not empty

        occupiedPos_.insert(temp_pos);
        
        auto new_ball = balls_.insert(std::make_pair(participant, getBall(x_temp,y_temp,initBallRadius_)));

         std::cout << "map elements_.size():  " << balls_.size() << std::endl;

        ((new_ball.first)->second)->setNick(nick);
        ((new_ball.first)->second)->setOwner(participant);

        id = ((new_ball.first)->second)->getId();

        //try
        //{
            auto new_element = elements_.insert(std::make_pair(id,(new_ball.first)->second )) ;       

          //  if ( new_element.second == false)
           //     throw collectionException();
        //}

        std::cout << "In addNewBall(): " << std::endl;
        std::cout << id << std::endl;
        occupiedPos_.insert(temp_pos);

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
        //send to enemies

        //insert new participant 
        participants_.insert(participant);

        updateParticipants();
             
    }

    ball_ptr GameBoard::getBall(double& x,double& y,double radius)
    {
        return std::make_shared<Ball>(x,y,radius);
    }

    void GameBoard::eraseBall(player_ptr participant)
    {
        std::cout << "erase Ball" <<  std::endl;
        ball_ptr ball;
        ball = balls_.at(participant);

        int id = ball->getId();
        std::pair<int,int> temp_pos = std::make_pair(ball->getX(),ball->getY());
        occupiedPos_.erase(temp_pos);

        std::string header_balls = "deleteBall:";

        std::cout << "deleting ball: " << id << std::endl;

        header_balls = header_balls + " " + boost::lexical_cast<std::string>(id);
        
        elements_.erase(id);
       
        balls_.erase(participant);

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

        //send delete ball frame to enemies
        Dataframe frm_balls;
        std::copy(header_balls.begin(), header_balls.end(), std::back_inserter(frm_balls.payload));

        deliver(frm_balls);
        

    }

    void GameBoard::sendGameState(player_ptr participant)
    {
        std::cout << " sending game state " << std::endl;
        int id;
        //send balls
        std::string header_balls = "gameStateBall:";

        balls_container::iterator j;

        std::cout << "gameState map elements_.size():  " << elements_.size() << std::endl;

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
            if ( !(i->second->getOwner() ) )
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
        
        std::vector<boost::uint8_t> temp;
        std::string rxss;
        std::string ryss;
        std::string temps;
        const char delimit_ = ':';
        const uint8_t delim = static_cast<uint8_t>(delimit_);
        const char delimitArg_ = ',';
        
        double radius;
        int id_source;
        double rx;
        double ry;

        double dist;
        double nx;
        double ny;
        double nradius;

        ball_ptr ball_source = balls_.at(source);

        radius = ball_source->getRadius();
        
        id_source = ball_source->getId();

        std::cout << "processMovement " << id_source <<std::endl;
        
        //parse incoming frame
        auto it_beg = std::find(msg.payload.begin(), msg.payload.end(),delim);
        std::copy(++it_beg, msg.payload.end(), std::back_inserter(temp));

        for(const auto& i : temp)
        {
            temps = temps +  boost::lexical_cast<std::string>(i);
        }

        int n = temps.find(delimitArg_);
        rxss = temps.substr(0,n);
        ryss = temps.substr(n+1,temps.size());

        rx = boost::lexical_cast<double>(rxss);
        ry = boost::lexical_cast<double>(ryss);
        
        //position update due to recent mass
        std::pair<int,int> temp_pos = std::make_pair(ball_source->getX(),ball_source->getY());
        occupiedPos_.erase(temp_pos);

        ball_source->xPosUpdate(rx);
        ball_source->yPosUpdate(ry);

        rx = ball_source->getX();
        ry = ball_source->getY();

        occupiedPos_.insert(std::make_pair(rx,ry));


        std::vector<int> neighbourId;

        //find neighbours within radius
        for (const auto & i: elements_ )
        {
            if( i.first != id_source )
            {
                nx = i.second->getX();
                ny = i.second->getY();
                dist = (rx-nx)*(rx-nx) + (ry-ny)*(ry-ny);
                if(dist > 0)
                    dist = std::sqrt(dist);
                else
                    return;
                if( dist <= radius)
                {
                    if(dist != radius)
                    {
                        nradius = i.second->getRadius();
                        if( nradius == foodRadius_ )
                        {
                            eraseFood(i.first);
                            addNFoodItem(onEatenNewItems_);   
                            ball_source->setRadius(radius + nradius);
                            ball_source->incFood();
                            break;
                        }
                        else
                        {
                            if(radius > nradius)
                            {
                               player_ptr owner = i.second->getOwner();
                               eraseBall(owner);
                               ball_source->setRadius(radius + nradius);
                               ball_source->incBall();
                               break;
                            }

                        }
                    }
                }
            }

        }

        std::string header = "ballUpdate:";
        
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getId());
        std::cout << "here" << std::endl;
        
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getX());
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getY());
        header = header + " " + boost::lexical_cast<std::string>(ball_source->getRadius());

        
        Dataframe frm;
        
        std::copy(header.begin(), header.end(), std::back_inserter(frm.payload));

        deliver(frm);

        std::cout << "processMovement end" << id_source << std::endl;
       

     }

} // namespace websocket