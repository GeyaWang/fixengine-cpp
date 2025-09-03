#include <fixengine/session/connection_handler.hpp>
#include <iostream>

namespace fix::session {
    void ConnectionHandler::stop() {
        connection_.close();
    }

    void ConnectionHandler::send(const std::string& msg) {
        try {
            connection_.write(msg);
        } catch (const std::runtime_error& error) {
            std::cerr << error.what() << ": " << msg << "\n";
        }
    }

    void ConnectionHandler::listen(const MsgHandler &on_msg) {
        connection_.listen(
            [this, on_msg](const std::string& msg) {
                on_msg(msg);
            }
        );
    }

    void ConnectionHandler::start(const MsgHandler &on_msg) {
        connection_.async_listen(
            [this, on_msg](const std::string& msg) {
                start(on_msg);
                on_msg(msg);
            }
        );
    }
}
