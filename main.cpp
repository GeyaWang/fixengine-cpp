#include <fixengine/utils/config.hpp>
#include <fixengine/session/session.hpp>

int main() {
    auto config = fix::utils::Config::load_file("C:/Users/geyae/CLionProjects/fixengine_cpp/config/config.yaml");
    fix::session::Session session(config);

    session.start();

    return 0;
}