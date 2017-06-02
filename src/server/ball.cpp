#include "ball.hpp"

namespace websocket{
	const std::string Ball::letters_("0123456789ABCDEF");

    double Ball::minJmp_{1};
    double Ball::maxJmp_{40};

    Ball::Ball(double & x,double & y,double & radius): Element(x,y,radius)
    {
    		getRandColor();
            std::cout << "construct ball " << this->getX() << " " << this->getY() << std::endl;

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
        //x_ = x_ + (scaleFact_*(1/(mass_))*x_vec);
        x_ = x_ + maxJmp_*x_vec;

    }

    void Ball::yPosUpdate(double y_vec)
    {
        //y_ = y_ + (scaleFact_*(1/(mass_))*y_vec);
        y_ = y_ + maxJmp_*y_vec;

        
    }   
}