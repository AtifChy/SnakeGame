#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Config {
  private:
    using json = nlohmann::json;
    static json config;
    static std::string filePath;

  public:
    Config();
    static void save();

    template <typename T>
    static void set(std::vector<std::string> keys, T value) {
        json* current = &config;
        for (int i = 0; i < keys.size(); i++) {
            if (i == keys.size() - 1) {
                (*current)[keys[i]] = value;
            } else {
                if (!(*current)[keys[i]].is_object()) {
                    (*current)[keys[i]] = json::object();
                }
                current = &(*current)[keys[i]];
            }
        }
    }

    template <typename T>
    static T get(std::vector<std::string> keys) {
        json* current = &config;
        for (const auto& key : keys) {
            if (current->is_object() && current->contains(key)) {
                current = &(*current)[key];
            } else {
                return T();
            }
        }
        return current->get<T>();
    }
};

extern Config instance;