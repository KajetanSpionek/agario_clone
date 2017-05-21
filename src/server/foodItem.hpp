/*!
*\file      FoodItem.hpp
*\author    
*\date      
*/
#ifndef FOODITEM_HPP
#define FOODITEM_HPP

#include <memory>
#include <iostream>
#include "element.hpp"

namespace websocket {

	class FoodItem : public Element
	{
	public:
		
		FoodItem(int & x, int & y, int radius, int& id);
		~FoodItem()
		{
		//std::cout << "delete food\n";
		} //= default;
        //FoodItem & FoodItem(const &FoodItem) = default;
	private:

	};

	typedef std::shared_ptr<FoodItem>  food_ptr;
} 	//namespace websocket

#endif //FOODITEM_H