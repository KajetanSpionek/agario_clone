/**
*\file      element.hpp
*\author    Wojciech Przybysz, Kajetan Spionek  
*           
*   
*/
#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <memory>
#include <iostream>
#include <atomic>
#include "player.hpp"

namespace websocket {

	class Element{
	public:
		
		Element(double & x,double & y,double & radius);
		virtual ~Element() {}
		Element(const Element &other) = default;
        Element & operator=(const Element& other) = default; 
       
		double getId() { return id_;}
        double getX() { return x_;}
        double getY() { return y_;}
        double getRadius() { return radius_; }

        void setX(const double&x) { x_ = x; }
        void setY(const double&y) { y_ = y; }
        void setRadius(double && radius) { radius_ = radius; }

        virtual player_ptr getOwner() = 0;
 
	private:

		const int id_;
		static int idCount_;

	 protected:

	 	double x_;
		double y_;
		double radius_;

	};

	typedef std::shared_ptr<Element>  element_ptr;
} 	//namespace websocket

#endif //ELEMENT_H
