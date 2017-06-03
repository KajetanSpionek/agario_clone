/*!
*\file      request.hpp
*\author    Wojciech Przybysz, Kajetan Spionek
*           A http request received from a client.
*/
#ifndef WEBSOCKET_HTTP_REQUEST_HPP
#define WEBSOCKET_HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace websocket {
    namespace http {

        struct Request
        {
            std::string method;
            std::string uri;
            int http_version_major;
            int http_version_minor;
            std::vector<Header> headers;
        };

    } // namespace http
} // namespace websocket

#endif // WEBSOCKET_HTTP_REQUEST_HPP