#include <fixengine/protocol/tag_value.hpp>

#define SOH '\x01'

namespace fix::protocol {
    std::string TagValue::str() const {
        return std::to_string(tag_) + "=" + value_ + SOH;
    }

    uint16_t TagValue::size() const {
        return str().size();
    }

    uint16_t TagValue::sum() const {
        uint16_t sum = 0;
        for (const char c : str()) {
            sum += static_cast<uint8_t>(c);
        }
        return sum;
    }

}
