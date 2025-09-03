#include <chrono>
#include <fixengine/protocol/message.hpp>
#include <sstream>
#include <iostream>

namespace fix::protocol {
    Message::Message(const std::string &msg) {
        fields_ = parse_(msg);
    }

    std::vector<Field> Message::parse_(const std::string &msg) {
        std::vector<Field> fields;
        size_t start = 0;
        size_t end = msg.find('\x01');

        while (end != std::string::npos) {
            std::string field = msg.substr(start, end - start);

            if (const size_t eq_pos = field.find('='); eq_pos != std::string::npos) {
                const int tag = std::stoi(field.substr(0, eq_pos));
                std::string value = field.substr(eq_pos + 1);

                if (tag == 8) begin_str_ = value;
                else if (tag == 35) msg_type_ = value;

                fields.emplace_back(tag, value);
            }

            start = end + 1;
            end = msg.find('\x01', start);
        }
        return fields;
    }

    void Message::remove_(int tag) {
        std::erase_if(
            fields_,
            [tag](const Field& tag_value) { return tag_value.tag() == tag; }
        );
    }

    size_t Message::body_size_() const {
        size_t sum = 0;
        for (const auto& field : fields_) {
            if (!(field.tag() == 8 || field.tag() == 9 || field.tag() == 10)) {
                sum += field.size();
            }
        }
        return sum;
    }

    int Message::checksum_() const {
        int sum = 0;
        for (const auto& field : fields_) {
            if (field.tag() != 10) {
                sum += field.sum();
            }
        }
        return sum % 256;
    }

    std::string Message::to_str_() const {
        std::ostringstream buffer;
        for (const auto& tag_value : fields_) {
            buffer << tag_value.str();
        }
        return buffer.str();
    }

    Message& Message::add(const int tag, const std::string& value) {
        fields_.emplace_back(tag, value);
        return *this;
    }

    Message& Message::add(const int tag, const int value) {
        return add(tag, std::to_string(value));
    }

    Message& Message::add(const std::string &msg) {
        auto new_fields = parse_(msg);
        fields_.insert(fields_.end(), new_fields.begin(), new_fields.end());
        return *this;
    }

    Message& Message::add_front(const int tag, const std::string& value) {
        fields_.emplace(fields_.begin(), tag, value);
        return *this;
    }

    Message& Message::add_front(const int tag, const int value) {
        return add_front(tag, std::to_string(value));
    }

    std::string Message::build() {
        remove_(35);
        remove_(9);
        remove_(8);
        remove_(10);

        // MsgType
        fields_.emplace(fields_.begin(), 35, msg_type_);
        // BodySize
        fields_.emplace(fields_.begin(), 9, std::to_string(body_size_()));
        // BeginStr
        fields_.emplace(fields_.begin(), 8, begin_str_);
        // Checksum
        add(10, std::format("{:03d}", checksum_()));

        return to_str_();
    }

    std::string Message::find(const int tag) const {
        for (const auto& field : fields_) {
            if (field.tag() == tag) {
                return field.value();
            }
        }
        return "";
    }

}
