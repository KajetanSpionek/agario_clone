/*!
*\file      Ball.hpp
*\author    
*\date      
*/
#ifndef BALL_HPP
#define BALL_HPP

#include "element.hpp"

class Ball : public Element
{
public:
    virtual void notify();
    void increaseMass(int add_mass); //increase mass = decrease velocity

private:
    double vX_;
    double vY_;

    //ball is always in the centre of the screen
    //visible distance defines how many pixels can
    //be seen from the edge of monitor
    double xVisibleDistance;
    double yVisibleDistance;
};

#endif //BALL_HPP