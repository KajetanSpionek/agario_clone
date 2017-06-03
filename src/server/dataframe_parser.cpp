#include "dataframe_parser.hpp"
#include "dataframe.hpp"

namespace websocket {

    DataframeParser::DataframeParser()
        : state_(FIN_OPCODE)
    {
    }

    void DataframeParser::reset()
    {
        state_ = FIN_OPCODE;
    }

    boost::uint8_t DataframeParser::getBits(char b, boost::uint8_t offset, boost::uint8_t count)
    {
        return (b >> offset) & ((1 << count) - 1);
    }

    boost::uint16_t DataframeParser::ntoh16(boost::uint16_t net16)
    {
        using namespace boost;
        static const int32_t num = 42;

        // Check the endianness.
        if (*reinterpret_cast<const char*>(&num) == num)
        {
            // Convert to the little-endian.
            uint16_t host16 = ((net16 & 0x00FFULL) <<  8) | ((net16 & 0xFF00ULL) >>  8);
            return host16;
        }

        return net16;
    }

    boost::uint16_t DataframeParser::hton16(boost::uint16_t host16)
    {
        using namespace boost;
        static const int32_t num = 42;

        // Check the endianness.
        if (*reinterpret_cast<const char*>(&num) == num)
        {
            // Convert to the big-endian.
            uint16_t net16 = ((host16 & 0x00FFULL) << 8) | ((host16 & 0xFF00ULL) >> 8);
            return net16;
        }

        return host16;
    }

    boost::uint64_t DataframeParser::ntoh64(boost::uint64_t net64)
    {
        using namespace boost;
        static const int32_t num = 42;

        // Check the endianness.
        if (*reinterpret_cast<const char*>(&num) == num)
        {
            // Convert to the little-endian.
            uint64_t host64 =
                ((net64 & 0x00000000000000FFULL) << 56) | 
                ((net64 & 0x000000000000FF00ULL) << 40) | 
                ((net64 & 0x0000000000FF0000ULL) << 24) | 
                ((net64 & 0x00000000FF000000ULL) <<  8) | 
                ((net64 & 0x000000FF00000000ULL) >>  8) | 
                ((net64 & 0x0000FF0000000000ULL) >> 24) | 
                ((net64 & 0x00FF000000000000ULL) >> 40) | 
                ((net64 & 0xFF00000000000000ULL) >> 56);

            return host64;
        }

        return net64;
    }

    boost::uint64_t DataframeParser::hton64(boost::uint64_t host64)
    {
        using namespace boost;
        static const int32_t num = 42;

        // Check the endianness.
        if (*reinterpret_cast<const char*>(&num) == num)
        {
            // Convert to the big-endian.
            uint64_t net64 =
                ((host64 & 0x00000000000000FFULL) << 56) | 
                ((host64 & 0x000000000000FF00ULL) << 40) | 
                ((host64 & 0x0000000000FF0000ULL) << 24) | 
                ((host64 & 0x00000000FF000000ULL) <<  8) | 
                ((host64 & 0x000000FF00000000ULL) >>  8) | 
                ((host64 & 0x0000FF0000000000ULL) >> 24) | 
                ((host64 & 0x00FF000000000000ULL) >> 40) | 
                ((host64 & 0xFF00000000000000ULL) >> 56);

            return net64;
        }

        return host64;
    }

    boost::tribool DataframeParser::consume(Dataframe& frame, boost::uint8_t input)
    {
        switch (state_)
        {
        case FIN_OPCODE:
            {
                frame.fin = getBits(input, 7, 1) == 1;

                switch (getBits(input, 0, 4))
                {
                case 0:
                    frame.opcode = Dataframe::CONTINUATION_FRAME;
                    break;
                case 0x1:
                    frame.opcode = Dataframe::TEXT_FRAME;
                    break;
                case 0x2:
                    frame.opcode = Dataframe::BINARY_FRAME;
                    break;
                case 0x8:
                    frame.opcode = Dataframe::CONNECTION_CLOSE;
                    break;
                case 0x9:
                    frame.opcode = Dataframe::PING;
                    break;
                case 0xA:
                    frame.opcode = Dataframe::PONG;
                    break;
                default:
                    frame.opcode = Dataframe::RESERVED;
                }

                state_ = MASK_PAYLOAD_LEN;

                return boost::indeterminate;
            }
        case MASK_PAYLOAD_LEN:
            {
                frame.mask = getBits(input, 7, 1) == 1;                    
                frame.payload_len = getBits(input, 0, 7);

                if (frame.payload_len == 0)
                {
                    return true;
                }
                else if (frame.payload_len == 126 || frame.payload_len == 127)
                {
                    state_ = EXTENDED_PAYLOAD_LEN1;
                }
                else
                {
                    if (frame.mask)
                    {
                        state_ = MASKING_KEY_1;
                    }
                    else
                    {
                        state_ = PAYLOAD;
                    }                     
                }

                return boost::indeterminate;
            }
        case EXTENDED_PAYLOAD_LEN1:
            {
                if (frame.payload_len == 126)
                {
                    frame.extended_payload_len16 = input;
                }
                else if (frame.payload_len == 127)
                {
                    frame.extended_payload_len64 = input;
                }

                state_ = EXTENDED_PAYLOAD_LEN2;

                return boost::indeterminate;
            }
        case EXTENDED_PAYLOAD_LEN2:
            {
                if (frame.payload_len == 126)
                {
                    boost::uint16_t temp = input;
                    temp = temp << 8;
                    frame.extended_payload_len16 |= temp;
                    frame.extended_payload_len16 = ntoh16(frame.extended_payload_len16);

                    if (frame.mask)
                    {
                        state_ = MASKING_KEY_1;
                    }
                    else
                    {
                        state_ = PAYLOAD;
                    }
                }
                else if (frame.payload_len == 127)
                {
                    boost::uint64_t temp = input;
                    temp = temp << 8;
                    frame.extended_payload_len64 |= temp;

                    state_ = EXTENDED_PAYLOAD_LEN3;
                }

                return boost::indeterminate;
            }

        case EXTENDED_PAYLOAD_LEN3:
            {
                boost::uint64_t temp = input;
                temp = temp << 16;
                frame.extended_payload_len64 |= temp;

                state_ = EXTENDED_PAYLOAD_LEN4;

                return boost::indeterminate;
            }

        case EXTENDED_PAYLOAD_LEN4:
            {
                boost::uint64_t temp = input;
                temp = temp << 24;
                frame.extended_payload_len64 |= temp;

                state_ = EXTENDED_PAYLOAD_LEN5;

                return boost::indeterminate;
            }
        case EXTENDED_PAYLOAD_LEN5:
            {
                boost::uint64_t temp = input;
                temp = temp << 32;
                frame.extended_payload_len64 |= temp;

                state_ = EXTENDED_PAYLOAD_LEN6;

                return boost::indeterminate;
            }
        case EXTENDED_PAYLOAD_LEN6:
            {
                boost::uint64_t temp = input;
                temp = temp << 40;
                frame.extended_payload_len64 |= temp;

                state_ = EXTENDED_PAYLOAD_LEN7;

                return boost::indeterminate;
            }
        case EXTENDED_PAYLOAD_LEN7:
            {
                boost::uint64_t temp = input;
                temp = temp << 48;
                frame.extended_payload_len64 |= temp;

                state_ = EXTENDED_PAYLOAD_LEN8;

                return boost::indeterminate;
            }
        case EXTENDED_PAYLOAD_LEN8:
            {
                boost::uint64_t temp = input;
                temp = temp << 56;
                frame.extended_payload_len64 |= temp;
                frame.extended_payload_len64 = ntoh64(frame.extended_payload_len64);

                if (frame.mask)
                {
                    state_ = MASKING_KEY_1;
                }
                else
                {
                    state_ = PAYLOAD;
                }

                return boost::indeterminate;
            }
        case MASKING_KEY_1:
            {
                frame.masking_key[0] = input;
                state_ = MASKING_KEY_2;

                return boost::indeterminate;
            }
        case MASKING_KEY_2:
            {
                frame.masking_key[1] = input;
                state_ = MASKING_KEY_3;

                return boost::indeterminate;
            }
        case MASKING_KEY_3:
            {
                frame.masking_key[2] = input;
                state_ = MASKING_KEY_4;

                return boost::indeterminate;
            }
        case MASKING_KEY_4:
            {
                frame.masking_key[3] = input;
                state_ = PAYLOAD;

                return boost::indeterminate;
            }
        case PAYLOAD:
            {
                boost::uint8_t mask = frame.masking_key[frame.payload.size() % 4];
                frame.payload.push_back(input ^ mask);

                if (frame.payload_len == 127)
                {
                    if (frame.payload.size() == frame.extended_payload_len64)
                        return true;
                    else
                        return boost::indeterminate;
                }
                else if (frame.payload_len == 126)
                {
                    if (frame.payload.size() == frame.extended_payload_len16)
                        return true;
                    else
                        return boost::indeterminate;
                }
                else
                {
                    if (frame.payload.size() == static_cast<std::vector<char>::size_type>(frame.payload_len))
                        return true;
                    else
                        return boost::indeterminate;
                }
            }

        default:
            return false;
        }
    }

} // namespace websocket