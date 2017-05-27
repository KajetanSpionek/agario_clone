#include "element.hpp"

namespace websocket {
	std::atomic<int> Element::idCount_;

	Element::Element(int & x,int & y,int & radius): id_(++idCount_), x_(x), y_(y),radius_(radius)
	{
		
	}

} 	//namespace websocket
