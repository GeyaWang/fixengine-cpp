#pragma once
#include <fixengine/network/tcp_connection.hpp>
#include <fixengine/protocol/parser.hpp>
#include <fixengine/utils/config.hpp>
#include "connection_handler.hpp"
#include "sequence_manager.hpp"

namespace fix::session {
    class Session {
        boost::asio::io_context io_context_;
        boost::asio::high_resolution_timer hb_timer_{io_context_};

        SequenceManager sequence_manager_;
        ConnectionHandler connection_handler_;
        protocol::Parser parser_;

        const utils::Config& config_;

        void run_hb_();
        void on_hb_();

        void on_receive_(const std::string& msg);

        void logon_();

    public:
        explicit Session(const utils::Config& config) : connection_handler_(io_context_, config), config_(config) {}

        void start();
        void disconnect();

    };
}
