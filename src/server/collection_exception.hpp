#ifndef WEBSOCKET_COLLECTION_EXCEPTION
#define WEBSOCKET_COLLECTION_EXCEPTION
#include <iostream>
#include <exception>
#include <string>

namespace websocket
{

	class collectionException 
	: public exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "GameBoard::elements_.insert() exception";
		}		

	};

} //namespace websocket

#endif // WEBSOCKET_COLLECTION_EXCEPTION