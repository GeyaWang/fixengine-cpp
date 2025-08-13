#pragma once
#include <cstdint>
#include <string>
#include <utility>

namespace fix::protocol {
    class TagValue {
        uint16_t tag_;
        std::string value_;

    public:
        TagValue(const uint16_t tag, std::string value) : tag_(tag), value_(std::move(value)) {}

        [[nodiscard]] uint16_t tag() const { return tag_; }
        [[nodiscard]] const std::string& value() const { return value_; }

        [[nodiscard]] std::string str() const;
        [[nodiscard]] uint16_t size() const;
        [[nodiscard]] uint16_t sum() const;
    };
}
