/*!
*\file      FoodItem.hpp
*\author    
*\date      
*/
#ifndef FOODITEM_HPP
#define FOODITEM_HPP

#include <memory>

namespace websocket {

	class FoodItem{
        int x_;
		int y_;
	public:
		
		FoodItem(int & x, int & y): x_(x), y_(y)
		{

		}
		//~FoodItem() = default;
        //FoodItem & FoodItem(const &FoodItem) = default;

        int getX() { return x_;}
        int getY() { return y_;}

	};

	typedef std::shared_ptr<FoodItem>  food_ptr;
} 	//namespace websocket

#endif //FOODITEM_H