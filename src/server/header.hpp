/*!
*\file      header.hpp
*\author    
*			Stucture for HTTP header
*/
#ifndef WEBSOCKET_HTTP_HEADER_HPP
#define WEBSOCKET_HTTP_HEADER_HPP

#include <string>

namespace websocket {
    namespace http {

        struct Header
        {
            std::string name;
            std::string value;
        };

    } // namespace http
} // namespace websocket

#endif // WEBSOCKET_HTTP_HEADER_HPP