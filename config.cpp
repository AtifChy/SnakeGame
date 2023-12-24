#include "config.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json Config::config;
std::string Config::filePath;

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

void Config::set(std::string key, std::string value) {
    config[key] = value;
}

void Config::set(std::string key, int value) {
    config[key] = value;
}

void Config::set(std::string key, float value) {
    config[key] = value;
}

void Config::set(std::string key, bool value) {
    config[key] = value;
}

std::string Config::getString(std::string key) {
    if (config[key].is_string()) {
        return config[key];
    }
    return "";
}

int Config::getInt(std::string key) {
    if (config[key].is_number()) {
        return config[key];
    }
    return 0;
}

float Config::getFloat(std::string key) {
    if (config[key].is_number()) {
        return config[key];
    }
    return 0.0f;
}

bool Config::getBool(std::string key) {
    if (config[key].is_boolean()) {
        return config[key];
    }
    return false;
}