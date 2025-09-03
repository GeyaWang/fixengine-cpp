#pragma once
#include <string>
#include <utility>
#include <vector>
#include "field.hpp"

namespace fix::protocol {
    class Message {
        std::vector<Field> fields_;
        std::string begin_str_;
        std::string msg_type_;

        std::vector<Field> parse_(const std::string& msg);

        void remove_(int tag);

        [[nodiscard]] size_t body_size_() const;
        [[nodiscard]] int checksum_() const;

        [[nodiscard]] std::string to_str_() const;

    public:
        Message(std::string  begin_str, std::string  msg_type) : begin_str_(std::move(begin_str)), msg_type_(std::move(msg_type)) {}
        explicit Message(const std::string& msg);

        Message& add(int tag, const std::string& value);
        Message& add(int tag, int value);
        Message& add(const std::string& msg);

        Message& add_front(int tag, const std::string& value);
        Message& add_front(int tag, int value);

        [[nodiscard]] std::string find(int tag) const;

        [[nodiscard]] std::string build();
    };
}
