#include <fixengine/utils/config.hpp>

namespace fix::utils {
    void Config::load(const std::string &filepath) {
        YAML::Node config = YAML::LoadFile(filepath);

        version = config["version"].as<std::string>();
        sender_comp_id = config["sender_comp_id"].as<std::string>();
        target_comp_id = config["target_comp_id"].as<std::string>();
        logon.host = config["logon"]["host"].as<std::string>();
        logon.port = config["logon"]["port"].as<std::string>();
        heartbeat_int = config["heartbeat_int"].as<int>();
    }

    Config Config::load_file(const std::string &filepath) {
        Config config;
        config.load(filepath);
        return config;
    }


}
