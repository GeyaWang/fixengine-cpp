#include <fixengine/protocol/field.hpp>

#define SOH '\x01'

namespace fix::protocol {
    std::string Field::str() const {
        return std::to_string(tag_) + "=" + value_ + SOH;
    }

    size_t Field::size() const {
        return str().size();
    }

    int Field::sum() const {
        int sum = 0;
        for (const char c : str()) {
            sum += static_cast<int>(c);
        }
        return sum;
    }

}
