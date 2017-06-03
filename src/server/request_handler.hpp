/*!
*\file      request_handler.hpp
*\author    Wojciech Przybysz, Kajetan Spionek
*           The handler for incoming http requests.
*/
#ifndef WEBSOCKET_HTTP_REQUEST_HANDLER_HPP
#define WEBSOCKET_HTTP_REQUEST_HANDLER_HPP

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

namespace websocket {
    namespace http {

        struct Reply;
        struct Request;

        class RequestHandler
            : private boost::noncopyable
        {
        public:
            /// Handle a request and produce a Reply.
            static void handleRequest(const Request& req, Reply& rep);

        private:
            /// Encode data using the SHA1 algorithm.
            static std::vector<unsigned char> toSha1(const std::string& s);

            /// Encode data using the Base64 algorithm.
            static std::string toBase64(const std::vector<unsigned char>& data);
        };

    } // namespace http
} // namespace websocket

#endif // WEBSOCKET_HTTP_REQUEST_HANDLER_HPP