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
        Ball(int & x,int & y,int & radius,int& id);
        ~Ball() {//
        //std::cout << "delete ball\n";
        } // = default;
        //Ball Ball(const & Ball) = default;

        std::string getColor() { return color_; }

    private:
        ///random color generator
        void getRandColor();
        //velocity
        int vX_;
        int vY_;

        int mass_;

        static const std::string letters_;

        std::string color_;

    };

    typedef std::shared_ptr<Ball>  ball_ptr;
}
#endif //BALL_HPP