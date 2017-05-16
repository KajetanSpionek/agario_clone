#ifndef WEBSOCKET_SESSION_HPP
#define WEBSOCKET_SESSION_HPP

#include <string>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_parser.hpp"
#include "dataframe.hpp"
#include "dataframe_parser.hpp"
#include "player.hpp"
#include "game_board.hpp"

namespace websocket {

        /// Represents a single connection from a client.
        class Session
            : public Player, 
            public boost::enable_shared_from_this<Session>,
            private boost::noncopyable
        {
        public:
            /// The Session states.
            enum state { UNDEFINED_STATE, READING_HANDSHAKE, BAD_REQUEST, WRITING_HANDSHAKE, PUMPING_MESSAGES } state_;

            /// Construct a connection with the given io_service.
            explicit Session(boost::asio::io_service& io_service, GameBoard& game_board);

            /// Get the socket associated with the connection.
            boost::asio::ip::tcp::socket& socket();

            /// Start the first asynchronous operation for the connection.
            void start();

        private:
            /// Handle completion of a read operation.
            void handle_read(const boost::system::error_code& e,
                std::size_t bytes_transferred);

            /// Handle completion of a write operation.
            void handle_write(const boost::system::error_code& e);

            /// Deliver a message to the client.
            void deliver(Dataframe msg);

            /// Get the client id.
            std::string getId();

            /// Strand to ensure the connection's handlers are not called concurrently.
            boost::asio::io_service::strand strand_;

            /// Socket for the connection.
            boost::asio::ip::tcp::socket socket_;

            /// The chat game_board where the messages will be posted.
            GameBoard& gameBoard_;

            /// The buffer for incoming data.
            boost::array<boost::uint8_t, 8192> buff_;

            /// The incoming http request.
            http::Request req_;

            /// The parser for the incoming http request.
            http::RequestParser reqParser_;

            /// The http Reply to be sent back to the client.
            http::Reply reply_;

            /// The parser for the incoming messages.
            DataframeParser frameParser_;

            /// The incoming message.
            Dataframe readFrame_;

            /// The messages to be sent to the client. 
            message_queue writeMsgs_;
        };

        typedef boost::shared_ptr<Session> session_ptr;

} // namespace websocket

#endif // WEBSOCKET_SESSION_HPP