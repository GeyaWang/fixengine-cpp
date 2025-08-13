#include <fixengine/session/session.hpp>

int main() {
    fix::session::Session session{"127.0.0.1", "1844"};
    session.start();

    return 0;
}
