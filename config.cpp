#include <iostream>
#include <fstream>
#include <json/json.h>
#include "config.h"

int MINIMUM_AVAILABLE_MEMORY = 0;
int MAX_USED_PERCENT = 0;
int MAX_TEMP = 0;
int MIN_TEMP = 0;

bool LoadConfig(const std::string& filename, int& min_memory, int& max_used_percent, int& max_temp, int& min_temp) {
    std::ifstream config_file(filename, std::ifstream::binary);
    if (!config_file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    Json::Value config;
    config_file >> config;

    if (config.isMember("MINIMUM_AVAILABLE_MEMORY") && config["MINIMUM_AVAILABLE_MEMORY"].isInt()) {
        min_memory = config["MINIMUM_AVAILABLE_MEMORY"].asInt();
    } else {
        std::cerr << "Missing or invalid MINIMUM_AVAILABLE_MEMORY in config file" << std::endl;
        return false;
    }

    if (config.isMember("MAX_USED_PERCENT") && config["MAX_USED_PERCENT"].isInt()) {
        max_used_percent = config["MAX_USED_PERCENT"].asInt();
    } else {
        std::cerr << "Missing or invalid MAX_USED_PERCENT in config file" << std::endl;
        return false;
    }

    if (config.isMember("MAX_TEMP") && config["MAX_TEMP"].isInt()) {
        max_temp = config["MAX_TEMP"].asInt();
    } else {
        std::cerr << "Missing or invalid MAX_TEMP in config file" << std::endl;
        return false;
    }

    if (config.isMember("MIN_TEMP") && config["MIN_TEMP"].isInt()) {
        min_temp = config["MIN_TEMP"].asInt();
    } else {
        std::cerr << "Missing or invalid MIN_TEMP in config file" << std::endl;
        return false;
    }

    return true;
}
