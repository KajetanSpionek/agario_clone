#include "request_parser.hpp"
#include "request.hpp"

namespace websocket {
    namespace http {

        RequestParser::RequestParser()
            : state_(METHOD_START)
        {
        }

        void RequestParser::reset()
        {
            state_ = METHOD_START;
        }

        boost::tribool RequestParser::consume(Request& req, char input)
        {
            switch (state_)
            {
            case METHOD_START:
                if (!is_char(input) || is_ctl(input) || is_tspecial(input))
                {
                    return false;
                }
                else
                {
                    state_ = METHOD;
                    req.method.push_back(input);
                    return boost::indeterminate;
                }
            case METHOD:
                if (input == ' ')
                {
                    state_ = URI;
                    return boost::indeterminate;
                }
                else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
                {
                    return false;
                }
                else
                {
                    req.method.push_back(input);
                    return boost::indeterminate;
                }
            case URI:
                if (input == ' ')
                {
                    state_ = HTTP_VERSION_H;
                    return boost::indeterminate;
                }
                else if (is_ctl(input))
                {
                    return false;
                }
                else
                {
                    req.uri.push_back(input);
                    return boost::indeterminate;
                }
            case HTTP_VERSION_H:
                if (input == 'H')
                {
                    state_ = HTTP_VERSION_T_1;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_T_1:
                if (input == 'T')
                {
                    state_ = HTTP_VERSION_T_2;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_T_2:
                if (input == 'T')
                {
                    state_ = HTTP_VERSION_P;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_P:
                if (input == 'P')
                {
                    state_ = HTTP_VERSION_SLASH;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_SLASH:
                if (input == '/')
                {
                    req.http_version_major = 0;
                    req.http_version_minor = 0;
                    state_ = HTTP_VERSION_MAJOR_START;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_MAJOR_START:
                if (is_digit(input))
                {
                    req.http_version_major = req.http_version_major * 10 + input - '0';
                    state_ = HTTP_VERSION_MAJOR;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_MAJOR:
                if (input == '.')
                {
                    state_ = HTTP_VERSION_MINOR_START;
                    return boost::indeterminate;
                }
                else if (is_digit(input))
                {
                    req.http_version_major = req.http_version_major * 10 + input - '0';
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_MINOR_START:
                if (is_digit(input))
                {
                    req.http_version_minor = req.http_version_minor * 10 + input - '0';
                    state_ = HTTP_VERSION_MINOR;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HTTP_VERSION_MINOR:
                if (input == '\r')
                {
                    state_ = EXPECTING_NEWLINE_1;
                    return boost::indeterminate;
                }
                else if (is_digit(input))
                {
                    req.http_version_minor = req.http_version_minor * 10 + input - '0';
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case EXPECTING_NEWLINE_1:
                if (input == '\n')
                {
                    state_ = HEADER_LINE_START;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HEADER_LINE_START:
                if (input == '\r')
                {
                    state_ = EXPECTING_NEWLINE_3;
                    return boost::indeterminate;
                }
                else if (!req.headers.empty() && (input == ' ' || input == '\t'))
                {
                    state_ = HEADER_LWS;
                    return boost::indeterminate;
                }
                else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
                {
                    return false;
                }
                else
                {
                    req.headers.push_back(Header());
                    req.headers.back().name.push_back(input);
                    state_ = HEADER_NAME;
                    return boost::indeterminate;
                }
            case HEADER_LWS:
                if (input == '\r')
                {
                    state_ = EXPECTING_NEWLINE_2;
                    return boost::indeterminate;
                }
                else if (input == ' ' || input == '\t')
                {
                    return boost::indeterminate;
                }
                else if (is_ctl(input))
                {
                    return false;
                }
                else
                {
                    state_ = HEADER_VALUE;
                    req.headers.back().value.push_back(input);
                    return boost::indeterminate;
                }
            case HEADER_NAME:
                if (input == ':')
                {
                    state_ = SPACE_BEFORE_HEADER_VALUE;
                    return boost::indeterminate;
                }
                else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
                {
                    return false;
                }
                else
                {
                    req.headers.back().name.push_back(input);
                    return boost::indeterminate;
                }
            case SPACE_BEFORE_HEADER_VALUE:
                if (input == ' ')
                {
                    state_ = HEADER_VALUE;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case HEADER_VALUE:
                if (input == '\r')
                {
                    state_ = EXPECTING_NEWLINE_2;
                    return boost::indeterminate;
                }
                else if (is_ctl(input))
                {
                    return false;
                }
                else
                {
                    req.headers.back().value.push_back(input);
                    return boost::indeterminate;
                }
            case EXPECTING_NEWLINE_2:
                if (input == '\n')
                {
                    state_ = HEADER_LINE_START;
                    return boost::indeterminate;
                }
                else
                {
                    return false;
                }
            case EXPECTING_NEWLINE_3:
                return (input == '\n');
            default:
                return false;
            }
        }

        bool RequestParser::is_char(int c)
        {
            return c >= 0 && c <= 127;
        }

        bool RequestParser::is_ctl(int c)
        {
            return (c >= 0 && c <= 31) || (c == 127);
        }

        bool RequestParser::is_tspecial(int c)
        {
            switch (c)
            {
            case '(': case ')': case '<': case '>': case '@':
            case ',': case ';': case ':': case '\\': case '"':
            case '/': case '[': case ']': case '?': case '=':
            case '{': case '}': case ' ': case '\t':
                return true;
            default:
                return false;
            }
        }

        bool RequestParser::is_digit(int c)
        {
            return c >= '0' && c <= '9';
        }

    } // namespace http
} // namespace websocket