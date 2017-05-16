#include "session.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

namespace websocket {


            Session::Session(boost::asio::io_service& io_service, GameBoard& game_board)
                : state_(UNDEFINED_STATE), strand_(io_service),
                socket_(io_service), gameBoard_(game_board)
            {
            }

            boost::asio::ip::tcp::socket& Session::socket()
            {
                return socket_;
            }

            void Session::start()
            {
                state_ = READING_HANDSHAKE;          
                socket_.async_read_some(boost::asio::buffer(buff_),
                    strand_.wrap(
                    boost::bind(&Session::handle_read, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
            }

            void Session::handle_read(const boost::system::error_code& error,
                std::size_t bytes_transferred)
            {
                if (!error)
                {
                    if (state_ == READING_HANDSHAKE)
                    {
                        boost::tribool result;
                        boost::tie(result, boost::tuples::ignore) = reqParser_.parse(
                            req_, buff_.data(), buff_.data() + bytes_transferred);

                        if (result)
                        {
                            http::RequestHandler::handleRequest(req_, reply_);

                            state_ = WRITING_HANDSHAKE;
                            boost::asio::async_write(socket_, reply_.to_buffers(),
                                strand_.wrap(
                                boost::bind(&Session::handle_write, shared_from_this(),
                                boost::asio::placeholders::error)));
                        }
                        else if (!result)
                        {
                            state_ = BAD_REQUEST;                        
                            reply_ = http::Reply::stock_reply(http::Reply::BAD_REQUEST);
                            boost::asio::async_write(socket_, reply_.to_buffers(),
                                strand_.wrap(
                                boost::bind(&Session::handle_write, shared_from_this(),
                                boost::asio::placeholders::error)));
                        }
                        else
                        {
                            socket_.async_read_some(boost::asio::buffer(buff_),
                                strand_.wrap(
                                boost::bind(&Session::handle_read, shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
                        }
                    }
                    else if (state_ == PUMPING_MESSAGES)
                    {
                        boost::tribool result;
                        boost::tie(result, boost::tuples::ignore) = frameParser_.parse(
                            readFrame_, buff_.data(), buff_.data() + bytes_transferred);

                        if (result)
                        {
                            if (readFrame_.opcode == Dataframe::TEXT_FRAME)
                            {
                                gameBoard_.deliver(readFrame_, shared_from_this());

                                readFrame_.payload.clear();
                                frameParser_.reset();

                                socket_.async_read_some(boost::asio::buffer(buff_),
                                    strand_.wrap(
                                    boost::bind(&Session::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred)));
                            }
                            else if (readFrame_.opcode == Dataframe::PONG ||
                                readFrame_.opcode == Dataframe::PING)
                            {
                                readFrame_.payload.clear();
                                frameParser_.reset();

                                socket_.async_read_some(boost::asio::buffer(buff_),
                                    strand_.wrap(
                                    boost::bind(&Session::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred)));
                            }
                            else
                            {
                                gameBoard_.leave(shared_from_this());
                            }
                        }
                        else if (!result)
                        {
                            gameBoard_.leave(shared_from_this());
                        }
                        else
                        {
                            socket_.async_read_some(boost::asio::buffer(buff_),
                                strand_.wrap(
                                boost::bind(&Session::handle_read, shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
                        }
                    }
                }
                else
                {
                    if (error != boost::asio::error::eof)
                        std::cerr << error.message() << std::endl;

                    gameBoard_.leave(shared_from_this());
                }

                // If an error occurs then no new asynchronous operations are started. This
                // means that all shared_ptr references to the connection object will
                // disappear and the object will be destroyed automatically after this
                // handler returns. The connection class's destructor closes the socket.
            }

            void Session::handle_write(const boost::system::error_code& error)
            {
                if (!error)
                {
                    if (state_ == WRITING_HANDSHAKE)
                    {
                        state_ = PUMPING_MESSAGES;

                        gameBoard_.join(shared_from_this());

                        socket_.async_read_some(boost::asio::buffer(buff_),
                                    strand_.wrap(
                                    boost::bind(&Session::handle_read, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred)));
                    }
                    else if (state_ == PUMPING_MESSAGES)
                    {
                        if (!writeMsgs_.empty())
                        {
                            writeMsgs_.pop_front();
                            if (!writeMsgs_.empty())
                            {
                                boost::asio::async_write(socket_,
                                    writeMsgs_.front().to_buffers(),
                                    strand_.wrap(boost::bind(&Session::handle_write, shared_from_this(),
                                    boost::asio::placeholders::error)));
                            }
                        }
                    }
                }
                else
                {
                    if (error != boost::asio::error::eof)
                        std::cerr << error.message() << std::endl;

                    gameBoard_.leave(shared_from_this());
                }
            }

            void Session::deliver(Dataframe msg)
            {
                bool write_in_progress = !writeMsgs_.empty();
                writeMsgs_.push_back(msg);
                if (!write_in_progress)
                {
                    boost::asio::async_write(socket_,
                        writeMsgs_.front().to_buffers(),
                        strand_.wrap(boost::bind(&Session::handle_write, shared_from_this(),
                        boost::asio::placeholders::error)));
                }
            }

            std::string Session::getId()
            {
                return socket_.is_open() ? "user" + boost::lexical_cast<std::string>(socket_.remote_endpoint().port()) : "user0";
            }


} // namespace websocket