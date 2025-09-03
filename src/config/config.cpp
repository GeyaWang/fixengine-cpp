#include <fixengine/config/config.hpp>
#include <fixengine/utils/sending_time.hpp>

namespace fix::config {
    bool Config::is_map_(const YAML::Node &node, const std::string &s) {
        return node.IsMap() && node[s].IsDefined();
    }

    bool Config::is_scalar_(const YAML::Node &node, const std::string &s) {
        return node.IsScalar() && node.as<std::string>() == s;
    }

    void Config::order_add_if_present_(const YAML::Node &node, action::OrderMsg &order, const std::string &field, int tag) {
        if (node[field].IsDefined()) {
            order.add(tag, node[field].as<std::string>());
        }
    }

    void Config::order_add_fields_(const YAML::Node &node, action::OrderMsg &order) {
        for (const auto& field : node) {
            order.add(field.first.as<int>(), field.second.as<std::string>());
        }
    }

    action::NewOrderSingle Config::get_new_order_single_(const YAML::Node &node) const {
        auto order_config = node["NewOrderSingle"];
        action::NewOrderSingle order{version, sender_comp_id, target_comp_id};
        order_add_if_present_(order_config, order, "ClOrdID", 11);
        order_add_if_present_(order_config, order, "HandlInst", 21);
        order_add_if_present_(order_config, order, "Symbol", 55);
        order_add_if_present_(order_config, order, "Side", 54);
        order_add_if_present_(order_config, order, "OrdType", 40);
        order_add_if_present_(order_config, order, "Price", 44);
        order_add_fields_(order_config["fields"], order);
        return order;
    }

    action::OrderCancel Config::get_order_cancel_(const YAML::Node &node) const {
        auto order_config = node["OrderCancel"];
        action::OrderCancel order{version, sender_comp_id, target_comp_id};
        order_add_if_present_(order_config, order, "OrigClOrdID", 41);
        order_add_if_present_(order_config, order, "ClOrdID", 11);
        order_add_if_present_(order_config, order, "Symbol", 55);
        order_add_if_present_(order_config, order, "Side", 54);
        order_add_fields_(order_config["fields"], order);
        return order;
    }

    action::CancelReplace Config::get_cancel_replace_(const YAML::Node &node) const {
        auto order_config = node["CancelReplace"];
        action::CancelReplace order{version, sender_comp_id, target_comp_id};
        order_add_if_present_(order_config, order, "OrigClOrdID", 41);
        order_add_if_present_(order_config, order, "ClOrdID", 11);
        order_add_if_present_(order_config, order, "HandlInst", 21);
        order_add_if_present_(order_config, order, "Symbol", 55);
        order_add_if_present_(order_config, order, "Side", 54);
        order_add_if_present_(order_config, order, "OrdType", 40);
        order_add_if_present_(order_config, order, "Price", 44);
        order_add_fields_(order_config["fields"], order);
        return order;
    }

    void Config::load_(const std::string &filepath) {
        YAML::Node config = YAML::LoadFile(filepath);

        version = config["version"].as<std::string>();
        sender_comp_id = config["sender_comp_id"].as<std::string>();
        target_comp_id = config["target_comp_id"].as<std::string>();
        socket.host = config["socket"]["host"].as<std::string>();
        socket.port = config["socket"]["port"].as<std::string>();
        heartbeat_int = config["heartbeat_int"].as<int>();

        auto actions_node = config["actions"];
        for (YAML::const_iterator actions_it = actions_node.begin(); actions_it != actions_node.end(); ++actions_it) {
            if (auto action = *actions_it; is_map_(action, "NewOrderSingle")) {
                actions.emplace_back(std::make_shared<action::OrderMsg>(get_new_order_single_(action)));
            } else if (is_map_(action, "OrderCancel")) {

                actions.emplace_back(std::make_shared<action::OrderMsg>(get_order_cancel_(action)));
            } else if (is_map_(action, "CancelReplace")) {
                actions.emplace_back(std::make_shared<action::OrderMsg>(get_cancel_replace_(action)));
            } else if (is_map_(action, "sleep")) {
                actions.emplace_back(std::make_shared<action::Sleep>(action["sleep"].as<int>()));
            } else if (is_scalar_(action, "logout")) {
                actions.emplace_back(std::make_shared<action::Logout>());
            }
        }
    }

    Config::Config(const std::string &filepath) {
        try {
            load_(filepath);
        } catch (YAML::BadFile&) {
            throw std::invalid_argument("Invalid filepath: " + filepath);
        }
    }
}
