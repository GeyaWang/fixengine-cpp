#include <fixengine/session/session.hpp>
#include <fixengine/protocol/message.hpp>
#include <fixengine/utils/time.hpp>
#include <iostream>

namespace fix::session {
    Session::Session(const config::Config &config) : connection_handler_(io_context_, config), config_(config) {
        for (const auto& action : config_.actions) {
            action_queue_.emplace(action);
        }
    }

    void Session::run_hb_() {
        hb_timer_.expires_after(std::chrono::seconds(config_.heartbeat_int));
        hb_timer_.async_wait(
            [this](const boost::system::error_code& error) {
                if (error) {
                    if (error != boost::asio::error::operation_aborted) {
                        std::cerr << "Error in hb timer: " << error.what();
                    }
                } else {
                    on_hb_();
                    run_hb_();
                }
            }
        );
    }

    void Session::on_hb_() {
        connection_handler_.send(
            protocol::Message{config_.version, "0"}
                .add(34, sequence_manager_.increment())
                .add(49, config_.sender_comp_id)
                .add(52, utils::time::get_UTC())
                .add(56, config_.target_comp_id)
                .build()
        );
    }

    void Session::on_msg_(const std::string& msg) {
        const protocol::Message parsed_msg{msg};
        const auto msg_type = parsed_msg.find(35);

        if (msg_type != "0") {
            utils::Logger::log(utils::LogType::RECEIVE, msg);
        }

        if ( msg_type == "3") {
            utils::Logger::log(utils::LogType::REJECT, parsed_msg.find(58));
        } else if (msg_type == "5") {
            utils::Logger::log(utils::LogType::STOP_LOGOUT, "Logged out");
            stop();
        } else if (msg_type == "8") {
            utils::Logger::log(utils::LogType::EXECUTION_REPORT, utils::Logger::get_ord_status(parsed_msg.find(150)));
        }
    }

    void Session::logon_() {
        const auto msg = protocol::Message{config_.version, "A"}
            .add(34, sequence_manager_.increment())
            .add(49, config_.sender_comp_id)
            .add(52, utils::time::get_UTC())
            .add(56, config_.target_comp_id)
            .add(98, 0)
            .add(108, config_.heartbeat_int)
            .add(141, "Y")
            .build();
        connection_handler_.send(msg);
        utils::Logger::log(utils::LogType::SEND, msg);
    }

    void Session::logout_() {
        const auto msg = protocol::Message{config_.version, "5"}
            .add(34, sequence_manager_.increment())
            .add(49, config_.sender_comp_id)
            .add(52, utils::time::get_UTC())
            .add(56, config_.target_comp_id)
            .build();
        connection_handler_.send(msg);
        utils::Logger::log(utils::LogType::SEND, msg);
    }

    void Session::stop() {
        hb_timer_.cancel();
        connection_handler_.stop();
    }

    void Session::do_actions() {
        while (!action_queue_.empty()) {
            const config::ActionPtr action = action_queue_.front();
            action_queue_.pop();

            switch (action->action_type) {
                case action::ActionType::SLEEP: {
                    auto sleep_action = dynamic_cast<action::Sleep*>(action.get());
                    if (sleep_action == nullptr) break;
                    sleep_timer_.expires_after(std::chrono::seconds(sleep_action->duration));
                    sleep_timer_.async_wait(
                        [this](const boost::system::error_code&) {
                            do_actions();
                        }
                    );
                    return;
                }
                case action::ActionType::ORDER_MSG: {
                    auto order_msg_action = dynamic_cast<action::OrderMsg*>(action.get());
                    if (order_msg_action == nullptr) break;
                    const auto msg = order_msg_action->build(sequence_manager_.increment());
                    connection_handler_.send(msg);
                    utils::Logger::log(utils::LogType::SEND, msg);
                    break;
                }
                case action::ActionType::LOGOUT: {
                    logout_();
                    break;
                }
                default: ;
            }
        }
    }

    void Session::start() {
        try {
            connection_handler_.connect();
            logon_();
            connection_handler_.start(msg_handler_);
            run_hb_();
            do_actions();
            io_context_.run();
        } catch (const std::runtime_error& error) {
            utils::Logger::log(utils::LogType::STOP_ERR, error.what());
            stop();
        }
    }
}
