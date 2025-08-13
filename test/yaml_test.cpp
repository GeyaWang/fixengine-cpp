#include <string>
#include <yaml-cpp/yaml.h>

int main() {
    YAML::Node config = YAML::LoadFile("C:/Users/geyae/CLionProjects/fixengine_cpp/config/config.yaml");
    return 0;
}