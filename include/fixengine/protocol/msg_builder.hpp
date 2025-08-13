#pragma once
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "tag_value.hpp"

namespace fix::protocol {
    class MsgBuilder {
        std::vector<TagValue> fields_;
        std::string begin_str_;
        std::string msg_type_;

        void add_front_(uint16_t tag, const std::string& value);
        void add_back_(uint16_t tag, const std::string& value);

        void remove_(uint16_t tag);
        void remove_(const std::string& value);

        [[nodiscard]] uint16_t body_size_() const;
        [[nodiscard]] uint16_t checksum_() const;

        [[nodiscard]] std::string to_str_() const;

    public:
        MsgBuilder(std::string  begin_str, std::string  msg_type) : begin_str_(std::move(begin_str)), msg_type_(std::move(msg_type)) {}

        MsgBuilder& add(uint16_t tag, const std::string& value);
        MsgBuilder& add(uint16_t tag, uint16_t value);

        [[nodiscard]] std::string build();
    };
}
