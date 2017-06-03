/*!
*\file      Ball.hpp
*\author    
*\date      
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

    class Ball 
    : public Element
    { 
    public:
        //void increaseMass(int add_mass); //increase mass = decrease velocity
        Ball(double & x,double & y,double & radius);
       // ~Ball() {//
        //    std::cout << "delete ball\n";
        //} // = default;
        
        Ball(const Ball &other) = default;
        Ball & operator=(const Ball& other) = default;
        void incBall() { ++ballEaten_; }
        void incFood() { ++foodEaten_; }
        int getBallNum() { return ballEaten_; }
        int getFoodNum() { return foodEaten_; }
        void xPosUpdate(double x_vec);
        void yPosUpdate(double y_vec);
        std::string & getNick() { return nick_;}
        void setNick(std::string nick) { nick_ = nick;}

        std::string getColor() { return color_; }
    private:

        ///random color generator
        void getRandColor();

        double mass_;

        static const std::string letters_;

        std::string color_;

        int ballEaten_;
        int foodEaten_;

        int v_;

        static double maxJmp_;

        std::string nick_;

    };

    typedef std::shared_ptr<Ball>  ball_ptr;
}
#endif //BALL_HPP