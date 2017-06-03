/**
*\file      dataframe_parser.hpp
*\author    
*           Parser for incoming dataframes.
*   
*/
#ifndef WEBSOCKET_DATAFRAME_PARSER_HPP
#define WEBSOCKET_DATAFRAME_PARSER_HPP

#include <boost/cstdint.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace websocket {

        struct Dataframe;

        class DataframeParser
        {
        public:
            /// Construct ready to parse the Dataframe.
            DataframeParser();

            /// Reset to initial parser state.
            void reset();

            template <typename InputIterator>
            boost::tuple<boost::tribool, InputIterator> parse(Dataframe& frame,
                InputIterator begin, InputIterator end)
            {
                while (begin != end)
                {
                    boost::tribool result = consume(frame, *begin++);
                    if (result || !result)
                        return boost::make_tuple(result, begin);
                }
                boost::tribool result = boost::indeterminate;
                return boost::make_tuple(result, begin);
            }

            /// Convert a uint16_t from the network byte order to the host byte order.
            static boost::uint16_t ntoh16(boost::uint16_t net16);

            /// Convert a uint16_t from the host byte order to the network byte order.
            static boost::uint16_t hton16(boost::uint16_t net16);

            /// Convert a uint64_t from the network byte order to the host byte order.
            static boost::uint64_t ntoh64(boost::uint64_t net64);

            /// Convert a uint64_t from the host byte order to the network byte order.
            static boost::uint64_t hton64(boost::uint64_t net64);

        private:
            /// Handle the next character of input.
            boost::tribool consume(Dataframe& frame, boost::uint8_t input);

            /// Get a number of bits at the specified offset.
            boost::uint8_t getBits(char b, boost::uint8_t offset, boost::uint8_t count);

            /// The current state of the parser.
            enum state
            {
                FIN_OPCODE,
                MASK_PAYLOAD_LEN,
                EXTENDED_PAYLOAD_LEN1,
                EXTENDED_PAYLOAD_LEN2,
                EXTENDED_PAYLOAD_LEN3,
                EXTENDED_PAYLOAD_LEN4,
                EXTENDED_PAYLOAD_LEN5,
                EXTENDED_PAYLOAD_LEN6,
                EXTENDED_PAYLOAD_LEN7,
                EXTENDED_PAYLOAD_LEN8,
                MASKING_KEY_1,
                MASKING_KEY_2,
                MASKING_KEY_3,
                MASKING_KEY_4,
                PAYLOAD
            } state_;
        };

} // namespace websocket

#endif // WEBSOCKET_DATAFRAME_PARSER_HPP