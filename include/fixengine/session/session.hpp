#pragma once
#include <fixengine/network/tcp_connection.hpp>
#include <fixengine/protocol/parser.hpp>
#include "connection_handler.hpp"
#include "sequence_manager.hpp"

namespace fix::session {
    class Session {
        boost::asio::io_context io_context_;
        boost::asio::high_resolution_timer hb_timer_{io_context_};

        SequenceManager sequence_manager_;
        ConnectionHandler connection_handler_;
        protocol::Parser parser_;

        uint16_t hb_int_s_ = 1;

        void run_hb_();
        void on_hb_();

        void on_receive_(const std::string& msg);

        void logon_();

    public:
        Session(const std::string& addr, const std::string& port) : connection_handler_(io_context_, addr, port) {}

        void start();
        void disconnect();

    };
}
