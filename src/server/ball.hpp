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

namespace websocket{
    class Ball 
    {
    public:
        //void increaseMass(int add_mass); //increase mass = decrease velocity
        Ball(int & x,int & y,int & radius,int& id);
        ~Ball() {//
        //std::cout << "delete ball\n";
        } // = default;
        //Ball Ball(const & Ball) = default;

        int getX() { return x_;}
        int getY() { return y_;}
        void setX(const int&x) { x_ = x; }
        void setY(const int&y) { y_ = y; }
        int getRadius() {return radius_;}
        int getId() {return Id_;}
        std::string getColor() { return color_; }

    private:
        ///random color generator
        void getRandColor();

        //id
        const int Id_;
        //position
        int x_;
        int y_;
        //velocity
        int vX_;
        int vY_;
        //radius
        int radius_;

        int mass_;

        static const std::string letters_;

        std::string color_;

    };
    typedef std::shared_ptr<Ball>  ball_ptr;
}
#endif //BALL_HPP