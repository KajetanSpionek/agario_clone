/*!
*\file      reply.hpp
*\author    Wojciech Przybysz, Kajetan Spionek
*           A http Reply to be sent to a client.
*/
#ifndef WEBSOCKET_HTTP_REPLY_HPP
#define WEBSOCKET_HTTP_REPLY_HPP

#include <vector>
#include <boost/asio.hpp>
#include "header.hpp"

namespace websocket {
    namespace http {

        struct Reply
        {
            /// The status of the Reply.
            enum status_type
            {
                SWITCHING_PROTOCOLS = 101,
                BAD_REQUEST = 400,
                INTERNAL_SERVER_ERROR = 500
            } status;

            /// The headers to be included in the Reply.
            std::vector<Header> headers;

            /// Convert the Reply into a vector of buffers. 
            std::vector<boost::asio::const_buffer> to_buffers();

            /// Get a stock Reply.
            static Reply stock_reply(status_type status);
        };

    } // namespace http
} // namespace websocket

#endif // WEBSOCKET_HTTP_REPLY_HPP