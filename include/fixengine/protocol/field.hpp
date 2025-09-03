#pragma once
#include <string>
#include <utility>

namespace fix::protocol {
    class Field {
        int tag_;
        std::string value_;

    public:
        Field(const int tag, std::string value) : tag_(tag), value_(std::move(value)) {}

        [[nodiscard]] int tag() const { return tag_; }
        [[nodiscard]] const std::string& value() const { return value_; }

        [[nodiscard]] std::string str() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] int sum() const;
    };
}
