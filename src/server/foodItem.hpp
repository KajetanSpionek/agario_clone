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
		
		FoodItem(double & x, double & y, double radius);
		~FoodItem()
		{
		//std::cout << "delete food\n";
		} //= default;
        FoodItem(const FoodItem & other) = default;
        FoodItem & operator=(const FoodItem& other) = default;
	private:

	};

	typedef std::shared_ptr<FoodItem>  food_ptr;
} 	//namespace websocket

#endif //FOODITEM_H