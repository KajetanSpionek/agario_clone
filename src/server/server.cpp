#include "server.hpp"
#include <iostream>
#include <vector>
//#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

namespace websocket {

        Server::Server(const std::string& address, const std::string& port)
            : io_service_(),
            signals_(io_service_),
            acceptor_(io_service_)
        {
            // Register to handle the signals that indicate when the Server should exit.
            // It is safe to register for the same signal multiple times in a program,
            // provided all registration for the specified signal is made through Asio.
            signals_.add(SIGINT);
            signals_.add(SIGTERM);
#if defined(SIGQUIT)
            signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
            signals_.async_wait(boost::bind(&Server::handle_stop, this));

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::resolver::query query(address, port);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();

            start_accept();
        }

        void Server::run()
        {
            io_service_.run();
        }

        void Server::start_accept()
        {
            new_session_.reset(new Session(io_service_, gameBoard_));
            acceptor_.async_accept(new_session_->socket(),
                boost::bind(&Server::handle_accept, this,
                boost::asio::placeholders::error));
        }

        void Server::handle_accept(const boost::system::error_code& error)
        {
            if (!error)
            {
                new_session_->start();
            }
            else
            {
                std::cerr << error.message() << std::endl;
            }

            start_accept();
        }

        void Server::handle_stop()
        {
            io_service_.stop();
        }

} // namespace websocket