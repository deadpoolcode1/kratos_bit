#include "config.h"

int MINIMUM_AVAILABLE_MEMORY = 0;
int MAX_USED_PERCENT = 0;

bool LoadConfig(const std::string& filename, int& min_memory, int& max_used_percent) {
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

    return true;
}
