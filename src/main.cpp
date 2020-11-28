#include <iostream>

#include "ListEngine.hpp"

int main() {

    std::string userHome = getenv("HOME");
    std::string cascadePath = "/.cascade/cascade.conf";
    std::string configPath = userHome + cascadePath;
    Config::getInstance().readFromConfigFile(configPath);

    ListEngine * engine = new ListEngine();

    try {
        engine->init();
    } catch(InvalidRatioException& e) {
        delete engine;
        std::cerr << "*** ERROR IN ENGINE INITIALIZATION ***" << std::endl;
        std::cerr << "InvalidRatioException: " << e.what() << std::endl;

        return 1;
    }

    engine->run();

    delete engine;

    return 0;

}
