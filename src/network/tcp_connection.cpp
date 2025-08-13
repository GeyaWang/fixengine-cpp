#include <fixengine/network/tcp_connection.hpp>
#include <iostream>

namespace fix::network {
    TCPConnection TCPConnection::create(boost::asio::io_context& io_context, const std::string& addr, const std::string& port) {
        tcp::resolver resolver(io_context);
        const auto endpoints = resolver.resolve(addr, port);

        return {io_context, endpoints};
    }

    void TCPConnection::connect() {
        try {
            boost::asio::connect(socket_, endpoints_);
        } catch (boost::system::system_error&) {
            std::cerr << "Failed to connect to endpoint\n";
            throw;
        }
    }

    void TCPConnection::close() {
        socket_.close();
    }

    void TCPConnection::write(const std::string &msg) {
        boost::asio::write(socket_, boost::asio::buffer(msg));
    }

    void TCPConnection::write(const std::string &msg, boost::system::error_code& error) {
        boost::asio::write(socket_, boost::asio::buffer(msg), error);
    }

    void TCPConnection::async_write(const std::string& msg, const ErrorHandler& handler) {
        boost::asio::async_write(socket_, boost::asio::buffer(msg), handler);
    }

    void TCPConnection::read_until_() {
        boost::asio::async_read_until(
            socket_,
            buffer_,
            "\x01",
            [this](const boost::system::error_code& error, std::size_t) {
                if (!error) {
                    std::istream is(&buffer_);
                    std::string str;
                    std::getline(is, str);
                    buffer_.consume(buffer_.size());
                    msg_handler_(str);

                    read_until_();
                }
            }
        );
    }

    void TCPConnection::start(MsgHandler on_msg) {
        msg_handler_ = std::move(on_msg);
        read_until_();
    }

}
