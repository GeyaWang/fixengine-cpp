#pragma once
#include <functional>
#include <vector>
#include "tag_value.hpp"

namespace fix::protocol {
    using MessageHandler = std::function<void(const std::string&)>;

    class Parser {
        std::vector<TagValue> fields_;

        void add_(uint16_t tag, const std::string& value);

    public:
        void parse(const std::string& msg);
    };
}
