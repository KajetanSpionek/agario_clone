#include "ball.hpp"

namespace websocket{
	const std::string Ball::letters_("0123456789ABCDEF");

    Ball::Ball(int & x,int & y,int & radius,int& id): Id_(id),x_(x), y_(y),radius_(radius)
    {
    		getRandColor();
            std::cout << "construct ball" << std::endl;
    }

    void Ball::getRandColor()
    {
    	std::srand(std::time(0));
    	int random_variable;

    	color_ = "#";
    	for(int i = 0; i < 6; i++)
    	{
    		random_variable = std::rand() % 16;
    		color_ = color_ +  letters_.at(random_variable);
    	}

    }
}