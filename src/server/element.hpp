#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <memory>
#include <iostream>
#include <atomic>

namespace websocket {

	class Element{
	public:
		
		Element(int & x,int & y,int & radius);
		~Element()
		{
		//std::cout << "delete food\n";
		} //= default;
        //FoodItem & FoodItem(const &FoodItem) = default;
		int getId() { return id_;}
        int getX() { return x_;}
        int getY() { return y_;}
        int getRadius() { return radius_; }
        void setX(const int&x) { x_ = x; }
        void setY(const int&y) { y_ = y; }
        void setRadius(int && radius) { radius_ = radius; }
	private:
		const int id_;
		static int idCount_;


	 protected:
	 	int x_;
		int y_;
		int radius_;

	};

	typedef std::shared_ptr<Element>  element_ptr;
} 	//namespace websocket

#endif //ELEMENT_H
