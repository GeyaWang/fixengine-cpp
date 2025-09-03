#pragma once
#include <yaml-cpp/yaml.h>
#include <fixengine/action/action.hpp>

namespace fix::config {
    using ActionPtr = std::shared_ptr<action::Action>;

    struct SocketConfig {
        std::string host;
        std::string port;
    };

    class Config {
        static bool is_map_(const YAML::Node& node, const std::string& s);
        static bool is_scalar_(const YAML::Node& node, const std::string& s);

        static void order_add_if_present_(const YAML::Node& node, action::OrderMsg& order, const std::string& field, int tag);
        static void order_add_fields_(const YAML::Node& node, action::OrderMsg& order);

        [[nodiscard]] action::NewOrderSingle get_new_order_single_(const YAML::Node& node) const;
        [[nodiscard]] action::OrderCancel get_order_cancel_(const YAML::Node& node) const;
        [[nodiscard]] action::CancelReplace get_cancel_replace_(const YAML::Node& node) const;

        void load_(const std::string& filepath);

    public:
        explicit Config(const std::string& filepath);

        std::string version;
        std::string sender_comp_id;
        std::string target_comp_id;
        SocketConfig socket;
        uint16_t heartbeat_int{};
        std::vector<ActionPtr> actions;
    };
}
