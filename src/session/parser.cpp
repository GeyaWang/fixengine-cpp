#include <fixengine/protocol/parser.hpp>
#include <iostream>

#define SOH '\x01'

namespace fix::protocol {
    void Parser::add_(uint16_t tag, const std::string &value) {
        fields_.emplace_back(tag, value);
    }

    void Parser::parse(const std::string &msg) {
        size_t start = 0;
        size_t end = msg.find(SOH);

        while (end != std::string::npos) {
            std::string field = msg.substr(start, end - start);

            if (const size_t eq_pos = field.find('='); eq_pos != std::string::npos) {
                const uint16_t tag = std::stoi(field.substr(0, eq_pos));
                std::string value = field.substr(eq_pos + 1);

                if (tag == 8) {
                    fields_.clear();
                } else if (tag == 10) {
                    std::cout << "Fix message parsed!\n";
                }

                add_(tag, value);
            }

            start = end + 1;
            end = msg.find(SOH, start);
        }
    }

}
