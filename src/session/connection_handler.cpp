#include <fixengine/session/connection_handler.hpp>
#include <iostream>

namespace {
    void send_err_handler(const std::string& msg, const boost::system::error_code& error) {
        if (!error) {
            std::cout << "Sent: \"" << msg << "\"\n";
        } else {
            std::cerr << "Failed to send message\n";
        }
    }
}

namespace fix::session {
    void ConnectionHandler::start(const network::MsgHandler &on_msg) {
        connection_.start(on_msg);
    }

    void ConnectionHandler::stop() {
        connection_.close();
    }

    void ConnectionHandler::send(const std::string &msg) {
        boost::system::error_code error;
        connection_.write(msg, error);
        send_err_handler(msg, error);
    }

    void ConnectionHandler::async_send(const std::string& msg, const std::function<void(boost::system::error_code, std::size_t)>& handler) {
        connection_.async_write(
            msg,
            [msg, handler](const boost::system::error_code& error, const std::size_t bytes_transferred) {
                send_err_handler(msg, error);
                handler(error, bytes_transferred);
            }
        );
    }

}
