#pragma once
#include <string>
#include <fixengine/protocol/message.hpp>
#include <fixengine/utils/sending_time.hpp>

namespace fix::action {
    enum class ActionType {
        SLEEP,
        ORDER_MSG,
        LOGOUT
    };

    struct Action {
        virtual ~Action() = default;
        explicit Action(ActionType action_type) : action_type(action_type) {}
        ActionType action_type;
    };

    struct Sleep : Action {
        explicit Sleep(const int time) : Action(ActionType::SLEEP), timeout(time) {}
        int timeout;
    };

    struct Logout : Action {
        Logout() : Action(ActionType::LOGOUT) {}
    };

    class OrderMsg : public Action {
    protected:
        protocol::Message msg_;
        std::string begin_str_;
        std::string msg_type_;

    public:
        explicit OrderMsg(
            const std::string& begin_str,
            const std::string& msg_type,
            const std::string& sender_comp_id,
            const std::string& target_comp_id
        ) : Action(ActionType::ORDER_MSG), msg_{begin_str, msg_type}, begin_str_(begin_str), msg_type_(msg_type) {
            add(49, sender_comp_id);
            add(56, target_comp_id);
            add(60, utils::current_time());
        }

        OrderMsg& add(int tag, const std::string& value);
        OrderMsg& add(int tag, int value);

        std::string build(int sequence);
    };

    struct NewOrderSingle final : OrderMsg {
        explicit NewOrderSingle(const std::string& begin_str, const std::string& sender_comp_id, const std::string& target_comp_id) :
            OrderMsg(begin_str, "D", sender_comp_id, target_comp_id) {}
    };

    struct OrderCancel final : OrderMsg {
        explicit OrderCancel(const std::string& begin_str, const std::string& sender_comp_id, const std::string& target_comp_id) :
            OrderMsg(begin_str, "F", sender_comp_id, target_comp_id) {}
    };

    struct CancelReplace final : OrderMsg {
        explicit CancelReplace(const std::string& begin_str, const std::string& sender_comp_id, const std::string& target_comp_id) :
            OrderMsg(begin_str, "G", sender_comp_id, target_comp_id) {}
    };
}
