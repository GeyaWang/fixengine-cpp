#pragma once
#include <string>
#include <chrono>

namespace fix::utils {
    inline std::string sending_time() {
        const auto now = std::chrono::system_clock::now();
        return std::format(
            "{:%Y%m%d-%H:%M:%S}.{:03d}",
            std::chrono::floor<std::chrono::seconds>(now),
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000
        );
    }
}
