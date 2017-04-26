#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <string>
#include <iostream>
#include <boost/asio.hpp>

#include <beast/core/placeholders.hpp>
#include <beast/core/streambuf.hpp>
#include <beast/websocket/stream.hpp>
#include <boost/lexical_cast.hpp>
#include <beast/core/to_string.hpp>
#include <boost/optional.hpp>
#include <boost/asio.hpp>
#include <string>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

class WebServer{
public:
    using error_code = beast::error_code;
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using adress_type = boost::asio::ip::address;
    using socket_type = boost::asio::ip::tcp::socket;

    void init();
    void run();
    void stop();
private:
    struct Ientity{
        template<typename Body, typename Fields>
        void operator()(beast::http::message<true, Body, Fields>& req) const{
            req.fields.replace("User-Agent","async_echo_client");
        }

        template<typename Body, typename Fields>
        void operator()(beast::http::message<false, Body, Fields>& resp) const{
            resp.fields.replace("Server","async_echo_client");
        }
    };

    template<typename NextLayer>
    class OptionsSet {
    private:
        class Callable {
        public:
            virtual ~Callable() = default;
            virtual void operator()(beast::websocket::stream<NextLayer>&) = 0;
        };

        template<typename T>
        class CallableImpl : public Callable{
        private:
            T t_;
        public:
            template<typename U>
            CallableImpl(U&& u) : t_(std::forward<U>(u)){
            }
            void operator()(beast::websocket::stream<NextLayer> &ws){
                t_(ws);
            }
        };

        template<typename Opt>
        class Lambda {
        private:
            Opt opt_;
        public:
            Lambda(Lambda &&) = default;
            Lambda(Lambda const&) = default;
            Lambda(Opt const &opt): opt_(opt){   
            }
            void operator()(beast::websocket::stream<NextLayer>&ws )const{
                ws.set_option(opt_);
            }
        };

        std::unordered_map<std::type_index,std::unique_ptr<Callable>> list_;

     public:
        template<class Opt>
        void setOption(Opt const& opt){
            std::unique_ptr<Callable> p;
            p.reset(new CallableImpl<Lambda<Opt>>{opt});
            list_[std::type_index{typeid(Opt)}] = std::move(p);
        }

        void setOptions(beast::websocket::stream<NextLayer>&ws){
            for(auto const & op : list_ ){
                (*op.second)(ws);
            }
        }

    };

    std::ostream* log_;
    boost::asio::io_service ios_;
    socket_type sock_;
    endpoint_type ep_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<std::thread> thread_;
    boost::optional<boost::asio::io_service::work> work_;
    OptionsSet<socket_type> opts_;

public:
    WebServer(WebServer const &) = delete;
    WebServer & operator=(WebServer const &) = delete;

    WebServer(std::ostream* log, std::size_t threads) : log_(log), sock_(ios_), acceptor_(ios_), work_(ios_) {
        opts_.setOption( beast::websocket::decorate(identity{}));
        thread_.reserve(threads);
        for(std::size_t i = 0; i < threads; ++i){
            thread_.emplace_back([&]{ios_.run();});
        }
    } 

    ~WebServer(){
        work_ = boost::none;
        error_code ec;
        ios_.dispatch( [&]{ acceptor_.close(ec);} );
        for( auto& t : thread_){
            t.join();
        }
    }

    endpoint_type localEndpoint() const{
        return acceptor_.local_endpoint();
    }

    template<typename Opt>
    void setOption(Opt const & opt){
        opts_.setOption(opt);
    }
    /*

    void open(endpoint_type const & ep, error_code & ec){
        acceptor_.open(ep.protocol(),ec);
        if(ec)
            return fail("open",ec);
        acceptor_.set_option(boost::asio::socket_base::reuse_address{true});
        acceptor_.bind(ep,ec);
        if(ec)
            return fail("bind",ec);
        acceptor_.listen(boost::asio::socket_base::max_connections, ec);
        if(ec)
            return fail("listen",ec);
        acceptor_.asyns_accept(sock_,ep_, std::bind(&WebServer::on_accept,this,beast::asio::placeholders::error));

    }
    */
private:

};

   


#endif //WEbSERVER_HPP