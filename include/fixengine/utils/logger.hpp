#pragma once
#include <string>

namespace fix::config {
    enum class LogType {
        SEND,
        RECEIVE,
        REJECT,
        EXECUTION_REPORT,
        STOP_ERR,
        STOP_LOGOUT
    };

    class Logger {
    public:
        static std::string format_fix_msg(const std::string& msg);

        static std::string get_ord_status(const std::string& status);

        static void log(const LogType& log_type, const std::string& msg);
    };
}
