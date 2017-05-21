/*
#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <memory>
#include <iostream>

namespace websocket {

	class Element{
	public:
		
		Element(int & x, int & y,int& id);
		~FoodItem()
		{
		//std::cout << "delete food\n";
		} //= default;
        //FoodItem & FoodItem(const &FoodItem) = default;
		int getId() { return Id_;}
        int getX() { return x_;}
        int getY() { return y_;}
	private:
		const int Id_;
        int x_;
		int y_;

	};

	typedef std::shared_ptr<FoodItem>  food_ptr;
} 	//namespace websocket

#endif //ELEMENT_H
*/