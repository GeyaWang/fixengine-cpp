#pragma once
#include <fixengine/network/tcp_connection.hpp>
#include <fixengine/config/config.hpp>
#include "connection_handler.hpp"
#include "sequence_manager.hpp"
#include <queue>

namespace fix::session {
    class Session {
        boost::asio::io_context io_context_;
        boost::asio::high_resolution_timer hb_timer_{io_context_};
        boost::asio::high_resolution_timer sleep_timer_{io_context_};

        SequenceManager sequence_manager_;
        ConnectionHandler connection_handler_;

        const config::Config& config_;
        std::queue<config::ActionPtr> action_queue_{};

        void run_hb_();
        void on_hb_();

        void on_msg_(const std::string& msg);
        std::function<void(std::string)> msg_handler_ = [this](const std::string& msg){ on_msg_(msg); };

        void logon_();
        void logout_();

        void do_actions();

    public:
        explicit Session(const config::Config& config);

        void start();
        void stop();

    };
}
