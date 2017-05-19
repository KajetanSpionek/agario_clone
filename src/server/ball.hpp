/*!
*\file      Ball.hpp
*\author    
*\date      
*/
#ifndef BALL_HPP
#define BALL_HPP

#include <memory>
#include <iostream>

namespace websocket{
    class Ball 
    {
    public:
        //void increaseMass(int add_mass); //increase mass = decrease velocity
        Ball(int & x,int & y,int & radius,int& id);
        ~Ball() {std::cout << "delete ball\n";} // = default;
        //Ball Ball(const & Ball) = default;

        int getX() { return x_;}
        int getY() { return y_;}
        int getRadius() {return radius_;}
        int getId() {return Id_;}
    private:
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

    };
    typedef std::shared_ptr<Ball>  ball_ptr;
}
#endif //BALL_HPP