#include "reply.hpp"
#include <string>

namespace websocket {
    namespace http {

        namespace status_strings {

            const std::string switching_protocols =
                "HTTP/1.1 101 Switching Protocols\r\n";
            const std::string bad_request =
                "HTTP/1.1 400 Bad Request\r\n";
            const std::string internal_server_error =
                "HTTP/1.1 500 Internal Server Error\r\n";

            boost::asio::const_buffer to_buffer(Reply::status_type status)
            {
                switch (status)
                {
                case Reply::SWITCHING_PROTOCOLS:
                    return boost::asio::buffer(switching_protocols);
                case Reply::BAD_REQUEST:
                    return boost::asio::buffer(bad_request);
                default:
                    return boost::asio::buffer(internal_server_error);
                }
            }

        } // namespace status_strings

        namespace misc_strings {

            const char name_value_separator[] = { ':', ' ' };
            const char crlf[] = { '\r', '\n' };

        } // namespace misc_strings

        std::vector<boost::asio::const_buffer> Reply::to_buffers()
        {
            std::vector<boost::asio::const_buffer> buffers;
            buffers.push_back(status_strings::to_buffer(status));

            for (std::size_t i = 0; i < headers.size(); ++i)
            {
                Header& h = headers[i];
                buffers.push_back(boost::asio::buffer(h.name));
                buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
                buffers.push_back(boost::asio::buffer(h.value));
                buffers.push_back(boost::asio::buffer(misc_strings::crlf));
            }

            buffers.push_back(boost::asio::buffer(misc_strings::crlf));

            return buffers;
        }

        Reply Reply::stock_reply(Reply::status_type status)
        {
            Reply rep;
            rep.status = status;

            return rep;
        }

    } // namespace http
} // namespace websocket