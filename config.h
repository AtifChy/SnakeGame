#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Config {
  private:
    json config;
    std::string filePath;

  public:
    Config();
    void save();
    void set(std::string key, std::string value);
    void set(std::string key, int value);
    void set(std::string key, float value);
    void set(std::string key, bool value);
    std::string getString(std::string key);
    int getInt(std::string key);
    float getFloat(std::string key);
    bool getBool(std::string key);
};

#endif // CONFIG_H