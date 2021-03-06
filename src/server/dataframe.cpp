#include "dataframe.hpp"
#include "dataframe_parser.hpp"

namespace websocket {

    Dataframe::Dataframe() : fin(true),
        opcode(TEXT_FRAME), mask(false), fin_opcode(0),
        mask_payload_len(0), payload_len(0), extended_payload_len16(0), extended_payload_len64(0) {}

    std::vector<boost::asio::const_buffer> Dataframe::to_buffers()
    {
        std::vector<boost::asio::const_buffer> buffers;

        if (fin)
        {
            fin_opcode |= 0x80;
        }

        switch (opcode)
        {
        case CONTINUATION_FRAME:    fin_opcode |= 0x0; break;
        case TEXT_FRAME:            fin_opcode |= 0x1; break;
        case BINARY_FRAME:          fin_opcode |= 0x2; break;
        case CONNECTION_CLOSE:      fin_opcode |= 0x8; break;
        case PING:                  fin_opcode |= 0x9; break;
        case PONG:                  fin_opcode |= 0xA; break;
        default:                    fin_opcode |= 0xF; break;
        }

        buffers.push_back(boost::asio::buffer(static_cast<const void*>(&fin_opcode), sizeof(fin_opcode)));

        if (payload.size() < 126)
        {
            mask_payload_len = static_cast<boost::uint8_t>(payload.size());
            buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
            buffers.push_back(boost::asio::buffer(payload));
        }
        else if (payload.size() < 65536)
        {
            mask_payload_len = 126;
            extended_payload_len16 = static_cast<boost::uint16_t>(payload.size());
            extended_payload_len16 = DataframeParser::hton16(extended_payload_len16);

            buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
            buffers.push_back(boost::asio::buffer(static_cast<const void*>(&extended_payload_len16), sizeof(extended_payload_len16)));
            buffers.push_back(boost::asio::buffer(payload));
        }
        else
        {
            mask_payload_len = 127;
            extended_payload_len64 = payload.size();
            extended_payload_len64 = DataframeParser::hton64(extended_payload_len64);

            buffers.push_back(boost::asio::buffer(static_cast<const void*>(&mask_payload_len), sizeof(mask_payload_len)));
            buffers.push_back(boost::asio::buffer(static_cast<const void*>(&extended_payload_len64), sizeof(extended_payload_len64)));
            buffers.push_back(boost::asio::buffer(payload));
        }

        return buffers;
    }

}