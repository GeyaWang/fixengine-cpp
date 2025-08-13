#pragma once
#include <fixengine/network/tcp_connection.hpp>

namespace fix::session {
    class ConnectionHandler {
        network::TCPConnection connection_;

    public:
        ConnectionHandler(boost::asio::io_context &io_context, const std::string& addr, const std::string& port) :
            connection_(network::TCPConnection::create(io_context, addr, port)) {}

        void connect() { connection_.connect(); }

        void start(const network::MsgHandler& on_msg);
        void stop();

        void send(const std::string& msg);
        void async_send(const std::string& msg, const std::function<void(boost::system::error_code, std::size_t)>& handler = [](const boost::system::error_code&, const std::size_t){});
    };
}
