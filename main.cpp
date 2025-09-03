#include <fixengine/config/config.hpp>
#include <fixengine/session/session.hpp>

int main(int argc, char* argv[]) {
    const std::vector<std::string> args(argv, argv + argc);

    std::string filepath;
    for (int i = 1; i < argc; i++) {
        if (const auto& arg = args[i]; arg == "-f" || arg == "--file") {
            if (i + 1 < argc) {
                filepath = args[i + 1];
            }
        }
    }

    if (filepath.empty()) {
        throw std::invalid_argument("required args: -f/-file");
    }

    fix::session::Session session(fix::utils::Config{filepath});
    session.start();
    return 0;
}