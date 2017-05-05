#ifndef WEBSOCKET_HTTP_REPLY_HPP
#define WEBSOCKET_HTTP_REPLY_HPP

#include <vector>
#include <boost/asio.hpp>
#include "header.hpp"

namespace websocket {
    namespace http {

        /// A http Reply to be sent to a client.
        struct Reply
        {
            /// The status of the Reply.
            enum status_type
            {
                SWITCHING_PROTOCOLS = 101,
                BAD_REQUEST = 400,
                INTERNAL_SERVER_ERROR = 500,
            } status;

            /// The headers to be included in the Reply.
            std::vector<Header> headers;

            /// Convert the Reply into a vector of buffers. The buffers do not own the
            /// underlying memory blocks, therefore the Reply object must remain valid and
            /// not be changed until the write operation has completed.
            std::vector<boost::asio::const_buffer> to_buffers();

            /// Get a stock Reply.
            static Reply stock_reply(status_type status);
        };

    } // namespace http
} // namespace websocket

#endif // WEBSOCKET_HTTP_REPLY_HPP