/**
*\file      ball.hpp
*\author    Wojciech Przybysz, Kajetan Spionek  
*           Class representing ball owned by a player.
*   
*/
#ifndef BALL_HPP
#define BALL_HPP

#include <memory>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <cmath>
#include "element.hpp"
#include "player.hpp"


namespace websocket{

    class Ball : public Element
    { 
    public:
        Ball(double & x,double & y,double & radius);

        ~Ball() = default;
        Ball(const Ball &other) = default;
        Ball & operator=(const Ball& other) = default;

        ///methods to provide statistics of a game
        void incBall() { ++ballEaten_; }
        void incFood() { ++foodEaten_; }

        int getBallNum() { return ballEaten_; }
        int getFoodNum() { return foodEaten_; }

        ///methods providing change of velocity with radius increase
        void xPosUpdate(double x_vec);
        void yPosUpdate(double y_vec);

        ///methods to set an unique nick for a ball
        std::string & getNick() { return nick_;}
        void setNick(std::string nick) { nick_ = nick;}

        ///method of random picking color for ball
        std::string getColor() { return color_; }

        virtual player_ptr getOwner() {return owner_;}
        void setOwner(player_ptr owner) { owner_ = owner; }

    private:

        ///random color generator
        void getRandColor();

        //static table used in color random picking
        static const std::string letters_;

        //attribute 
        std::string color_;

        //attributes for game statistics
        int ballEaten_;
        int foodEaten_;

        //scaling factor for velocity
        static double maxJmp_;

        //unique nick of a player
        std::string nick_;

        //game map properties
        int mapX_;
        int mapY_;

        player_ptr owner_;

    };

    typedef std::shared_ptr<Ball>  ball_ptr;
}
#endif //BALL_HPP