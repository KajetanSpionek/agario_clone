/*!
*\file      Ball.hpp
*\author    
*\date      
*/
#ifndef BALL_HPP
#define BALL_HPP

namespace websocket{
    class Ball 
    {
    public:
        
        //void increaseMass(int add_mass); //increase mass = decrease velocity
        Ball(int & x,int & y,int & radius): x_(x), y_(y),radius_(radius)
        {
            
        }
        Ball() = default;


        int getX() { return x_;}
        int getY() { return y_;}
        int getRadius() {return radius_;}
    private:
        //position
        int x_;
        int y_;
        //velocity
        int vX_;
        int vY_;
        //radius
        int radius_;

        int mass_;

        
        //double xVisibleDistance;
        //double yVisibleDistance;
    };
    typedef std::shared_ptr<Ball>  ball_ptr;
}
#endif //BALL_HPP