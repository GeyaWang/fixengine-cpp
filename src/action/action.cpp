#include <fixengine/action/action.hpp>
#include <fixengine/utils/sending_time.hpp>

namespace fix::action {
    OrderMsg& OrderMsg::add(const int tag, const std::string& value) {
        msg_.add(tag, value);
        return *this;
    }

    OrderMsg& OrderMsg::add(const int tag, const int value) {
        msg_.add(tag, value);
        return *this;
    }

    std::string OrderMsg::build(const int sequence) {
        msg_.add_front(52, config::current_time());
        msg_.add_front(34, sequence);

        return msg_.build();
    }
}
