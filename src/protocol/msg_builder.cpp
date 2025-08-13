#include <chrono>
#include <fixengine/protocol/msg_builder.hpp>
#include <sstream>
#include <iostream>

namespace fix::protocol {
    void MsgBuilder::add_front_(uint16_t tag, const std::string &value) {
        fields_.emplace(fields_.begin(), tag, value);
    }

    void MsgBuilder::add_back_(uint16_t tag, const std::string &value) {
        fields_.emplace_back(tag, value);
    }

    void MsgBuilder::remove_(const std::string &value) {
        std::erase_if(
            fields_,
            [&value](const TagValue& tag_value) { return tag_value.value() == value; }
        );
    }

    void MsgBuilder::remove_(uint16_t tag) {
        std::erase_if(
            fields_,
            [tag](const TagValue& tag_value) { return tag_value.tag() == tag; }
        );
    }

    uint16_t MsgBuilder::body_size_() const {
        uint16_t sum = 0;
        for (const auto& tag_value : fields_) {
            if (!(tag_value.tag() == 8 || tag_value.tag() == 9 || tag_value.tag() == 10)) {
                sum += tag_value.size();
            }
        }
        return sum;
    }

    uint16_t MsgBuilder::checksum_() const {
        uint16_t sum = 0;
        for (const auto& tag_value : fields_) {
            if (tag_value.tag() != 10) {
                sum += tag_value.sum();
            }
        }
        return sum % 256;
    }

    std::string MsgBuilder::to_str_() const {
        std::ostringstream buffer;
        for (const auto& tag_value : fields_) {
            buffer << tag_value.str();
        }
        return buffer.str();
    }


    MsgBuilder& MsgBuilder::add(const uint16_t tag, const std::string &value) {
        fields_.emplace_back(tag, value);
        return *this;
    }

    MsgBuilder &MsgBuilder::add(const uint16_t tag, const uint16_t value) {
        return add(tag, std::to_string(value));
    }

    std::string MsgBuilder::build() {
        // MsgType
        remove_(35);
        add_front_(35, msg_type_);

        // BodySize
        remove_(9);
        add_front_(9, std::to_string(body_size_()));

        // BeginString
        remove_(8);
        add_front_(8, begin_str_);

        // Checksum
        remove_(10);
        add_back_(10, std::format("{:03d}", checksum_() % 256));

        return to_str_();
    }

}
