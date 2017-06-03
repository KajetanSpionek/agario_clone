#include "ball.hpp"
#include "game_board.hpp"
#include <cmath>

namespace websocket{
	const std::string Ball::letters_("0123456789ABCDEF");

    double Ball::maxJmp_{1000};

    Ball::Ball(double & x,double & y,double & radius): Element(x,y,radius)
    {
    		getRandColor();
            std::cout << "construct ball " << this->getX() << " " << this->getY() << std::endl;

            nick_ = "Mike";

            ballEaten_ = 0;
            foodEaten_ = 0;

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

    void Ball::xPosUpdate(double x_vec)
    {
        x_ = x_ + (1/radius_)*maxJmp_*x_vec;
        if( x_ < 0)
            x_ = 0;
        else if( x_ > 2999 )
            x_ = 2999;

    }

    void Ball::yPosUpdate(double y_vec)
    {
        y_ = y_ + (1/radius_)*maxJmp_*y_vec ;
        if( y_ < 0)
            y_ = 0;
        else if( y_ > 2999 )
            y_ = 2999;

        
    }   
}