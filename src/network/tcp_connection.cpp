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
            throw std::runtime_error("Failed to connect to endpoints");
        }
    }

    void TCPConnection::close() {
        socket_.close();
    }

    void TCPConnection::write(const std::string &msg) {
        boost::system::error_code error;
        boost::asio::write(socket_, boost::asio::buffer(msg), error);

        if (error == boost::asio::error::bad_descriptor) {
            throw std::runtime_error("Failed to send message");
        }
    }

    void TCPConnection::async_write(const std::string& msg, const ErrorHandler& handler) {
        boost::asio::async_write(socket_, boost::asio::buffer(msg), handler);
    }

    std::string TCPConnection::read_buffer_() {
        std::istream is(&buffer_);
        std::string str;
        std::getline(is, str);
        buffer_.consume(buffer_.size());
        return str;
    }

    void TCPConnection::listen(const MsgHandler& on_msg) {
        boost::system::error_code error;
        boost::asio::read_until(socket_, buffer_, "\x01", error);

        if (error == boost::asio::error::eof) {
            throw std::runtime_error("Connection closed by server");
        }

        on_msg(read_buffer_());
    }

    void TCPConnection::async_listen(const MsgHandler &on_msg) {
        boost::asio::async_read_until(
            socket_,
            buffer_,
            "\x01",
            [this, on_msg](const boost::system::error_code& error, std::size_t) {
                if (!error) {
                    on_msg(read_buffer_());
                }
            }
        );
    }
}
