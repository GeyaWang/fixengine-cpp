#include <fixengine/utils/logger.hpp>
#include <algorithm>
#include <iostream>

namespace fix::utils {
    std::string Logger::format_fix_msg(const std::string &msg) {
        std::string new_msg = msg;
        std::ranges::replace(new_msg, '\x01', '|');
        return new_msg;
    }

    std::string Logger::get_ord_status(const std::string &status) {
        if (status == "0") return "New";
        if (status == "1") return "Partially Filled";
        if (status == "2") return "Filled";
        if (status == "3") return "Done for day";
        if (status == "4") return "Cancelled";
        if (status == "5") return "Replaced";
        if (status == "6") return "Pending Cancel";
        if (status == "7") return "Stopped";
        if (status == "8") return "Rejected";
        if (status == "9") return "Suspended";
        if (status == "A") return "Pending New";
        if (status == "B") return "Calculated";
        if (status == "C") return "Expired";
        if (status == "D") return "Accepted for bidding";
        if (status == "E") return "Pending replace";
        return "Unknown order status";
    }

    void Logger::log(const LogType& log_type, const std::string& msg) {
        switch (log_type) {
            case LogType::SEND:
                std::cout << "SEND: " << format_fix_msg(msg) << "\n";
                break;
            case LogType::RECEIVE:
                std::cout << "RECEIVE: " << format_fix_msg(msg) << "\n";
                break;
            case LogType::REJECT:
                std::cerr << "REJECT: " << msg << "\n";
                break;
            case LogType::EXECUTION_REPORT:
                std::cout << "EXECUTION_REPORT: " << msg << "\n";
                break;
            case LogType::STOP_ERR:
                std::cerr << "SESSION_STOP: " << msg << "\n";
                break;
            case LogType::STOP_LOGOUT:
                std::cout << "SESSION_STOP: " << msg << "\n";
                break;
            default: ;
        }
    }

}
