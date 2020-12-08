#include <iostream>
#include <sys/stat.h>

#include "ListEngine.hpp"

void printKeybindingsHelp() {
    std::cout << "cascade - keybindings and modes" << std::endl << std::endl;

    std::cout << "--=== NORMAL MODE ===--" << std::endl << std::endl;

    std::cout << "  q   - quit cascade" << std::endl;
    std::cout << "  j,k - change item focus up and down" << std::endl;
    std::cout << "  J,K - change section focus up and down" << std::endl;
    std::cout << "  g,G - jump to top or bottom of section" << std::endl;
    std::cout << "  e,E - edit item name/section title" << std::endl;
    std::cout << "  n,N - add new item/section" << std::endl;
    std::cout << "  d,D - delete focused item/section" << std::endl;
    std::cout << "  m   - enter move mode" << std::endl;
    std::cout << "  c   - cycle focused section color" << std::endl;
    std::cout << "  s   - save any unsaved changes" << std::endl << std::endl;

    std::cout << "--=== MOVE MODE ===--" << std::endl << std::endl;

    std::cout << "  q   - quit cascade" << std::endl;
    std::cout << "  j,k - move focused item up and down" << std::endl;
    std::cout << "  J,K - move focused section up and down" << std::endl;
    std::cout << "  m   - exit move mode" << std::endl;
    std::cout << "  s   - save any unsaved changes" << std::endl;
}

void printConfigHelp() {
    std::cout << "cascade - configuration location and options" << std::endl << std::endl;

    std::cout << "Configuration is stored in the file found at ~/.cascade/cascade.conf" << std::endl;
    std::cout << "Options should be specified like '<key> = <value>'" << std::endl;
    std::cout << "Configurable options are as follows:" << std::endl << std::endl;

    std::cout << "  MasterList - The default list cascade will open if no list file is specified on startup." << std::endl;
    std::cout << "               This is set to ~/.cascade/master.todo by default." << std::endl << std::endl;

    std::cout << "  DefaultSectionColor - The default color of new sections when they are created." << std::endl;
    std::cout << "                        This is set to white by default." << std::endl;
}

void printListHelp() {
    std::cout << "cascade - list file format specifications" << std::endl << std::endl;

    std::cout << "List files must follow a specific format in order to be properly parsed and rendered." << std::endl;
    std::cout << "The files must meet the following criteria:" << std::endl << std::endl;

    std::cout << "  - The file must not be empty." << std::endl;
    std::cout << "  - The first line of the file must be a section line." << std::endl;
    std::cout << "  - Section lines must be of the format [Title] : ColorCode" << std::endl;
    std::cout << "  - There must be a blank line between sections." << std::endl;
    std::cout << "  - There cannot be a blank line between items of the same section." << std::endl;
}

void printHelpInfo(int argc, char ** argv) {
    if(argc == 2) {
        std::cout << "cascade - A simple and customizable todo list for the terminal" << std::endl << std::endl;

        std::cout << "USAGE: cascade [LIST]" << std::endl;
        std::cout << "Here LIST is an optional argument containing a path to a .todo file" << std::endl;
        std::cout << "If LIST is not specified, the master list defined in ~/.cascade/cascade.conf is used" << std::endl;
        std::cout << "Calling 'cascade -h' or 'cascade --help' will show this dialog" << std::endl << std::endl;

        std::cout << "You can also call the help dialog with the following commands:" << std::endl;
        std::cout << "cascade -h keybindings :: learn about keybindings and modes in cascade" << std::endl;
        std::cout << "cascade -h config :: learn about configuration options for cascade" << std::endl;
        std::cout << "cascade -h list :: learn about format specifications for .todo files" << std::endl;
    } else {
        std::string topic = argv[2];
        if(topic == "keybindings") {
            printKeybindingsHelp();
        } else if(topic == "config") {
            printConfigHelp();
        } else if(topic == "list") {
            printListHelp();
        } else {
            std::cout << "Unknown help option '" << topic << "'. Correct help options include:" << std::endl << std::endl;

            std::cout << "keybindings" << std::endl;
            std::cout << "config" << std::endl;
            std::cout << "list" << std::endl;
        }
    }
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
    if(pathExists(directoryPath) != 0) {
        int permissions = S_IRUSR | S_IWUSR | S_IXUSR;
        mkdir(directoryPath.c_str(), permissions);
    }
}

void ensureFileExists(std::string path, std::string filename) {
    if(pathExists(filename) != 0) {
        std::ofstream configFile(path);
        configFile << "MasterList = ~/.cascade/master.todo" << std::endl;
        configFile << "DefaultSectionColor = white" << std::endl;
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
            printHelpInfo(argc, argv);
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
        if(listPath == "") {
            std::cerr << "User has not specified a master list file in the configuration file." << std::endl;
            std::cerr << "Configuration file should be located in " + getDefaultConfigPath() << std::endl;
            std::cerr << "To see how to specify a master list, run 'cascade -h config'" << std::endl;

            return 1;
        }
    }
    
    ListEngine * engine = new ListEngine(listPath);

    try {
        engine->init();
        engine->run();
    } catch(InvalidFileException& e) {
        delete engine;
        std::cerr << "*** ERROR IN ENGINE ***" << std::endl;
        std::cerr << "InvalidFileException: " << e.what() << std::endl;
        std::cerr << "File located at: " << listPath << std::endl;

        return 1;
    } catch(InvalidRatioException& e) {
        delete engine;
        std::cerr << "*** ERROR IN ENGINE ***" << std::endl;
        std::cerr << "InvalidRatioException: " << e.what() << std::endl;

        return 1;
    } catch(std::exception& e) {
        delete engine;
        std::cerr << "*** ERROR IN ENGINE ***" << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;

        return 1;
    }

    delete engine;

    return 0;

}
