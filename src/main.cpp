#include <iostream>
#include <sys/stat.h>

#include "ListEngine.hpp"

void printHelpInfo() {
    std::cout << "cascade - A simple and customizable todo list for the terminal" << std::endl;
    std::cout << std::endl;
    std::cout << "USAGE: cascade [LIST]" << std::endl;
    std::cout << "Here LIST is an optional argument containing a path to a .todo file" << std::endl;
    std::cout << "If LIST is not specified, the master list defined in ~/.cascade/cascade.conf is used" << std::endl;
    std::cout << "Calling 'cascade -h' or 'cascade --help' will show this dialog" << std::endl;
}

std::string getDefaultConfigPath() {
    std::string userHome = getenv("HOME");
    std::string cascadePath = "/.cascade/cascade.conf";
    std::string configPath = userHome + cascadePath;

    return configPath;
}

bool pathExists(std::string path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void ensureDirectoryExists(std::string directoryPath) {
    if(!pathExists(directoryPath)) {
        int permissions = S_IRUSR | S_IWUSR | S_IXUSR;
        mkdir(directoryPath.c_str(), permissions);
    }
}

void ensureFileExists(std::string path, std::string filename) {
    if(!pathExists(filename)) {
        std::ofstream configFile(path);
        configFile << "MasterList = ~/.cascade/master.todo" << std::endl;
        configFile.close();
    }
}

void ensureConfigExists(std::string path) {
    size_t split = path.find_last_of('/');
    std::string directory = path.substr(0, split);
    std::string configFile = path.substr(split + 1);

    ensureDirectoryExists(directory);
    ensureFileExists(path, configFile);
}

void setupConfig() {
    std::string configPath = getDefaultConfigPath();
    ensureConfigExists(configPath);
    Config::getInstance().readFromConfigFile(configPath);
}

int main(int argc, char ** argv) {

    std::string listPath;
    bool userPassedList = false;

    // Check command line arguments
    if(argc > 1) {
        std::string arg1 = argv[1];
        if(arg1 == "-h" || arg1 == "--help") {
            printHelpInfo();
            return 0;
        } else {
            // Use this as the list to read from
            userPassedList = true;
        }
    }

    setupConfig();

    if(userPassedList) {
        listPath = argv[1];
    } else {
        listPath = Config::getInstance().getValueFromKey("MasterList");
    }
    
    ListEngine * engine = new ListEngine(listPath);

    try {
        engine->init();
        engine->run();
    } catch(InvalidFileException& e) {
        delete engine;
        std::cerr << "*** ERROR IN ENGINE ***" << std::endl;
        std::cerr << "InvalidFileException: " << e.what() << std::endl;

        return 1;
    } catch(InvalidRatioException& e) {
        delete engine;
        std::cerr << "*** ERROR IN ENGINE ***" << std::endl;
        std::cerr << "InvalidRatioException: " << e.what() << std::endl;

        return 1;
    }

    delete engine;

    return 0;

}
