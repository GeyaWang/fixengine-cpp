#pragma once
#include <yaml-cpp/yaml.h>

namespace fix::utils {
    struct LogonConfig {
        std::string host;
        std::string port;
    };

    class Config {
    public:
        void load(const std::string& filepath);
        static Config load_file(const std::string& filepath);

        std::string version;
        std::string sender_comp_id;
        std::string target_comp_id;
        LogonConfig logon;
        uint16_t heartbeat_int{};
    };
}
