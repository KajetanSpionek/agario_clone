/*!
*\file      request_parser.hpp
*\author    Wojciech Przybysz, Kajetan Spionek
*           Parser for incoming requests.
*/
#ifndef WEBSOCKET_SERVER_REQUEST_PARSER_HPP
#define WEBSOCKET_SERVER_REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace websocket {
    namespace http {

        struct Request;

        class RequestParser
        {
        public:
            /// Construct ready to parse the Request method.
            RequestParser();

            /// Reset to initial parser state.
            void reset();

            /// Parse some data. 
            template <typename InputIterator>
            boost::tuple<boost::tribool, InputIterator> parse(Request& req,
                InputIterator begin, InputIterator end)
            {
                while (begin != end)
                {
                    boost::tribool result = consume(req, *begin++);
                    if (result || !result)
                        return boost::make_tuple(result, begin);
                }
                boost::tribool result = boost::indeterminate;
                return boost::make_tuple(result, begin);
            }

        private:
            /// Handle the next character of input.
            boost::tribool consume(Request& req, char input);

            /// Check if a byte is an HTTP character.
            static bool is_char(int c);

            /// Check if a byte is an HTTP control character.
            static bool is_ctl(int c);

            /// Check if a byte is defined as an HTTP tspecial character.
            static bool is_tspecial(int c);

            /// Check if a byte is a digit.
            static bool is_digit(int c);

            /// The current state of the parser.
            enum state
            {
                METHOD_START,
                METHOD,
                URI,
                HTTP_VERSION_H,
                HTTP_VERSION_T_1,
                HTTP_VERSION_T_2,
                HTTP_VERSION_P,
                HTTP_VERSION_SLASH,
                HTTP_VERSION_MAJOR_START,
                HTTP_VERSION_MAJOR,
                HTTP_VERSION_MINOR_START,
                HTTP_VERSION_MINOR,
                EXPECTING_NEWLINE_1,
                HEADER_LINE_START,
                HEADER_LWS,
                HEADER_NAME,
                SPACE_BEFORE_HEADER_VALUE,
                HEADER_VALUE,
                EXPECTING_NEWLINE_2,
                EXPECTING_NEWLINE_3
            } state_;
        };

    } // namespace http
} // namespace websocket

#endif // WEBSOCKET_SERVER_REQUEST_PARSER_HPP