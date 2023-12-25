#include "config.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

json Config::config;
std::string Config::filePath;
Config instance;

Config::Config() {
    filePath = "config.json";
    std::ifstream file(filePath);
    if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
        std::ofstream newFile(filePath);
        newFile << "{}";
        newFile.close();
        file.open(filePath);
    }
    file >> config;
}

void Config::save() {
    std::ofstream file(filePath);
    file << config.dump(4);
    file.close();
}