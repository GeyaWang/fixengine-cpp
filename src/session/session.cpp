#include <fixengine/session/session.hpp>
#include <fixengine/protocol/msg_builder.hpp>
#include <fixengine/utils/sending_time.hpp>

#include <iostream>

namespace fix::session {
    void Session::run_hb_() {
        hb_timer_.expires_after(std::chrono::seconds(hb_int_s_));
        hb_timer_.async_wait(
            [this](const boost::system::error_code&) {
                on_hb_();
                run_hb_();
            }
        );
    }

    void Session::on_hb_() {
        connection_handler_.async_send(
            protocol::MsgBuilder("FIX.4.4", "0")
            .add(34, sequence_manager_.get())
            .add(49, "FIXSIM-CLIENT")
            .add(52, utils::sending_time())
            .add(56, "FIXSIM-SERVER")
            .build()
        );
        sequence_manager_.increment();
    }

    void Session::on_receive_(const std::string &msg) {
        std::cout << "Received: \"" << msg << "\"\n";
        parser_.parse(msg);
    }

    void Session::logon_() {
        connection_handler_.send(
        protocol::MsgBuilder("FIX.4.4", "A")
            .add(34, sequence_manager_.get())
            .add(49, "FIXSIM-CLIENT")
            .add(52, utils::sending_time())
            .add(56, "FIXSIM-SERVER")
            .add(98, 0)
            .add(108, hb_int_s_)
            .add(141, "Y")
            .build()
        );
        sequence_manager_.increment();
    }

    void Session::disconnect() {
        connection_handler_.stop();
    }

    void Session::start() {
        try {
            connection_handler_.connect();
            logon_();
            connection_handler_.start([this](const std::string& msg){ on_receive_(msg); });
            run_hb_();
            io_context_.run();
        } catch (const boost::system::system_error& error) {
            disconnect();
            std::cerr << "Stopping session\n" << error.what();
            throw;
        }
    }
}
