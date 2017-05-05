#ifndef WEBSOCKET_DATAFRAME_HPP
#define WEBSOCKET_DATAFRAME_HPP

#include <vector>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace websocket {

    // A structure to hold websocket frame data. 
    struct Dataframe
    {
        Dataframe();

        bool fin;
        enum operation_code { CONTINUATION_FRAME, TEXT_FRAME, BINARY_FRAME, CONNECTION_CLOSE, PING, PONG, RESERVED } opcode;
        bool mask;
        boost::int8_t fin_opcode;
        boost::int8_t mask_payload_len;
        boost::int8_t payload_len;
        boost::uint16_t extended_payload_len16;
        boost::uint64_t extended_payload_len64;
        boost::array<boost::uint8_t, 4> masking_key;
        std::vector<boost::uint8_t> payload;

        /// Convert the Dataframe into a vector of buffers. 
        std::vector<boost::asio::const_buffer> to_buffers();
    };

} // namespace websocket

#endif // WEBSOCKET_DATAFRAME_HPP