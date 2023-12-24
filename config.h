#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Config {
  private:
    static json config;
    static std::string filePath;

  public:
    Config();
    static void save();
    static void set(std::string key, std::string value);
    static void set(std::string key, int value);
    static void set(std::string key, float value);
    static void set(std::string key, bool value);
    static std::string getString(std::string key);
    static int getInt(std::string key);
    static float getFloat(std::string key);
    static bool getBool(std::string key);
};

#endif // CONFIG_H