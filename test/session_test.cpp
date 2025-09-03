#include <fixengine/session/session.hpp>

int main() {
    const fix::config::Config config {"C:/Users/geyae/CLionProjects/fixengine_cpp/config.yaml"};
    fix::session::Session session(config);
    session.start();

    return 0;
}
