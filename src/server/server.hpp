#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "game_board.hpp"
#include "session.hpp"
#include "request_handler.hpp"

namespace websocket {

        /// The top-level class of the chat server.
        class Server
            : private boost::noncopyable
        {
        public:
            /// Construct the Server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            explicit Server(const std::string& address, const std::string& port);

            /// Run the Server's io_service loop.
            void run();

        private:
            /// Initiate an asynchronous accept operation.
            void start_accept();

            /// Handle completion of an asynchronous accept operation.
            void handle_accept(const boost::system::error_code& e);

            /// Handle a Request to stop the Server.
            void handle_stop();

            /// The io_service used to perform asynchronous operations.
            boost::asio::io_service io_service_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The next connection to be accepted.
            session_ptr new_session_;

            /// The handler for all incoming requests.
            http::RequestHandler request_handler_;

            /// The chat game_board.
            GameBoard gameBoard_;
        };

} // namespace websocket


#endif // WEBSOCKET_SERVER_HPP