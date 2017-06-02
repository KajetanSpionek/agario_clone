#include "element.hpp"

namespace websocket {
	int Element::idCount_;

	Element::Element(double & x,double & y,double & radius): id_(++idCount_), x_(x), y_(y),radius_(radius)
	{
		
	}

} 	//namespace websocket
