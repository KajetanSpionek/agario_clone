#include "element.hpp"

namespace websocket {
	

	Element::Element(int & x,int & y,int & radius): id_(++idCount_), x_(x), y_(y),radius_(radius)
	{
		
	}

} 	//namespace websocket
