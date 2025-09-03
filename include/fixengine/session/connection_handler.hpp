#pragma once
#include <fixengine/network/tcp_connection.hpp>
#include <fixengine/config/config.hpp>
#include <fixengine/utils/logger.hpp>

namespace fix::session {
    using MsgHandler = std::function<void(std::string)>;

    class ConnectionHandler {
        network::TCPConnection connection_;

    public:
        ConnectionHandler(boost::asio::io_context &io_context, const utils::Config& config) :
            connection_(network::TCPConnection::create(io_context, config.socket.host, config.socket.port)) {}

        void connect() { connection_.connect(); }
        void stop();

        void send(const std::string& msg);
        void listen(const MsgHandler& on_msg);

        void start(const MsgHandler& on_msg);
    };
}
