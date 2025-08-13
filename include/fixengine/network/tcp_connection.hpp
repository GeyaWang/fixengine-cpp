#pragma once
#include <memory>
#include <boost/asio.hpp>
#include <utility>


namespace fix::network {
    using boost::asio::ip::tcp;
    using ErrorHandler = std::function<void(boost::system::error_code, std::size_t)>;
    using MsgHandler = std::function<void(std::string)>;

    class TCPConnection {
        tcp::socket socket_;
        tcp::resolver::results_type endpoints_;
        boost::asio::streambuf buffer_;

        MsgHandler msg_handler_;

        TCPConnection(boost::asio::io_context &io_context, tcp::resolver::results_type endpoints):
            socket_(io_context), endpoints_(std::move(endpoints)) {}

        void read_until_();

    public:
        static TCPConnection create(boost::asio::io_context& io_context, const std::string& addr, const std::string& port);

        tcp::socket& get_socket() { return socket_; }

        void connect();
        void close();

        void write(const std::string& msg);
        void write(const std::string& msg, boost::system::error_code& error);
        void async_write(const std::string& msg, const ErrorHandler& handler);

        void start(MsgHandler on_msg);
    };
}
