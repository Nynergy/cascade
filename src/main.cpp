#include "ListEngine.hpp"

int main() {

    std::string userHome = getenv("HOME");
    std::string cascadePath = "/.cascade/cascade.conf";
    std::string configPath = userHome + cascadePath;
    Config::getInstance().readFromConfigFile(configPath);

    ListEngine * engine = new ListEngine();
    engine->init();
    engine->run();

    delete engine;

    return 0;

}
