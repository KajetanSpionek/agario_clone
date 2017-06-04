/*!
*\file      foodItem.hpp
*\author    Wojciech Przybysz, Kajetan Spionek
*
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
		~FoodItem() = default;
        FoodItem(const FoodItem & other) = default;
        FoodItem & operator=(const FoodItem& other) = default;

        virtual player_ptr getOwner() {return nullptr;} 

	private:

	};

	typedef std::shared_ptr<FoodItem>  food_ptr;
} 	//namespace websocket

#endif //FOODITEM_H