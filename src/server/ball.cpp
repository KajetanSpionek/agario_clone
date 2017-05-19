#include "ball.hpp"

namespace websocket{
    Ball::Ball(int & x,int & y,int & radius,int& id): Id_(id),x_(x), y_(y),radius_(radius)
    {
            std::cout << "construct ball" << std::endl;
    }
}