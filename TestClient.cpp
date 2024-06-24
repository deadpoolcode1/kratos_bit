#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <json/json.h>
#include <string>
#include <optional>
#include <map>
#include "config.h"

#define SOCKET_PATH "/tmp/uds_socket"
#define CONFIG_FILE "config.json"

std::string sendCommand(const std::string &command) {
    int client_fd;
    struct sockaddr_un addr;
    char buf[1024];

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket error");
        return "";
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect error");
        close(client_fd);
        return "";
    }

    write(client_fd, command.c_str(), command.size());
    int n = read(client_fd, buf, sizeof(buf));
    if (n > 0) {
        buf[n] = '\0';
        close(client_fd);
        return std::string(buf);
    }
    close(client_fd);
    return "";
}

void performTest(const std::string& test) {
    Json::Value root;
    std::string command;

    if (test == "READ_LATEST_RESULTS") {
        root["command"] = "READ_LATEST_RESULTS";
    } else if (test == "PERFORM_IBIT") {
        root["command"] = "PERFORM_IBIT";
    } else if (test == "CHANGE_CBIT_TIME") {
        root["command"] = "CHANGE_CBIT_TIME";
        root["new_time"] = 20; // Example new time value
    } else if (test == "READ_CBIT_TIME") {
        root["command"] = "READ_CBIT_TIME";
    } else if (test == "READ_PBIT") {
        root["command"] = "READ_PBIT";
    } else if (test == "PERFORM_TEST_TEMP") {
        root["command"] = "PERFORM_TEST_TEMP";
    } else if (test == "PERFORM_TEST_FREESPACE") {
        root["command"] = "PERFORM_TEST_FREESPACE";
    } else if (test == "PERFORM_TEST_I2C") {
        root["command"] = "PERFORM_TEST_I2C";
    } else if (test == "PERFORM_TEST_RTC") {
        root["command"] = "PERFORM_TEST_RTC";
    } else if (test == "PERFORM_TEST_GPIO") {
        root["command"] = "PERFORM_TEST_GPIO";
    } else if (test == "PERFORM_TEST_IRQ") {
        root["command"] = "PERFORM_TEST_IRQ";
    } else if (test == "PERFORM_TEST_UART") {
        root["command"] = "PERFORM_TEST_UART";
    } else if (test == "PERFORM_TEST_SPI") {
        root["command"] = "PERFORM_TEST_SPI";
    } else if (test == "PERFORM_TEST_RGMII") {
        root["command"] = "PERFORM_TEST_RGMII";
    } else if (test == "PERFORM_TEST_MEMORY") {
        root["command"] = "PERFORM_TEST_MEMORY";
    } else if (test == "PERFORM_TEST_FPGA") {
        root["command"] = "PERFORM_TEST_FPGA";
    } else {
        std::cerr << "Unknown test: " << test << std::endl;
        return;
    }

    command = Json::writeString(Json::StreamWriterBuilder(), root);
    std::cout << "Sending: " << command << std::endl;
    std::cout << "Response: " << sendCommand(command) << std::endl;
}

void performAllTests() {
    performTest("READ_LATEST_RESULTS");
    performTest("PERFORM_IBIT");
    performTest("CHANGE_CBIT_TIME");
    performTest("READ_CBIT_TIME");
    performTest("READ_PBIT");
    performTest("PERFORM_TEST_TEMP");
    performTest("PERFORM_TEST_FREESPACE");
    performTest("PERFORM_TEST_I2C");
    performTest("PERFORM_TEST_RTC");
    performTest("PERFORM_TEST_GPIO");
    performTest("PERFORM_TEST_IRQ");
    performTest("PERFORM_TEST_UART");
    performTest("PERFORM_TEST_SPI");
    performTest("PERFORM_TEST_RGMII");
    performTest("PERFORM_TEST_MEMORY");
    performTest("PERFORM_TEST_FPGA");
}

void showHelp() {
    std::cout << "Usage: TestClient_evb [option]\n"
              << "Options:\n"
              << "  READ_LATEST_RESULTS    Reads the latest results\n"
              << "  PERFORM_IBIT           Performs IBIT\n"
              << "  CHANGE_CBIT_TIME       Changes CBIT time\n"
              << "  READ_CBIT_TIME         Reads CBIT time\n"
              << "  READ_PBIT              Reads PBIT results\n"
              << "  PERFORM_TEST_TEMP      Performs Temperature test\n"
              << "  PERFORM_TEST_FREESPACE Performs Free Space test\n"
              << "  PERFORM_TEST_I2C       Performs I2C test\n"
              << "  PERFORM_TEST_RTC       Performs RTC test\n"
              << "  PERFORM_TEST_GPIO      Performs GPIO test\n"
              << "  PERFORM_TEST_IRQ       Performs IRQ test\n"
              << "  PERFORM_TEST_UART      Performs UART test\n"
              << "  PERFORM_TEST_SPI       Performs SPI test\n"
              << "  PERFORM_TEST_RGMII     Performs RGMII test\n"
              << "  PERFORM_TEST_MEMORY    Performs Memory test\n"
              << "  PERFORM_TEST_FPGA      Performs FPGA test\n"
              << "  -h, --help             Show this help message\n";
}

std::optional<std::string> parseCommandLineArguments(int argc, char* argv[]) {
    if (argc < 2) {
        return std::nullopt;
    }

    std::map<std::string, std::string> validCommands = {
        {"READ_LATEST_RESULTS", "Reads the latest results"},
        {"PERFORM_IBIT", "Performs IBIT"},
        {"CHANGE_CBIT_TIME", "Changes CBIT time"},
        {"READ_CBIT_TIME", "Reads CBIT time"},
        {"READ_PBIT", "Reads PBIT results"},
        {"PERFORM_TEST_TEMP", "Performs Temperature test"},
        {"PERFORM_TEST_FREESPACE", "Performs Free Space test"},
        {"PERFORM_TEST_I2C", "Performs I2C test"},
        {"PERFORM_TEST_RTC", "Performs RTC test"},
        {"PERFORM_TEST_GPIO", "Performs GPIO test"},
        {"PERFORM_TEST_IRQ", "Performs IRQ test"},
        {"PERFORM_TEST_UART", "Performs UART test"},
        {"PERFORM_TEST_SPI", "Performs SPI test"},
        {"PERFORM_TEST_RGMII", "Performs RGMII test"},
        {"PERFORM_TEST_MEMORY", "Performs Memory test"},
        {"PERFORM_TEST_FPGA", "Performs FPGA test"},
        {"-h", "Show help"},
        {"--help", "Show help"}
    };

    std::string test = argv[1];
    if (validCommands.find(test) == validCommands.end()) {
        std::cerr << "Invalid test name provided. Valid tests are:" << std::endl;
        for (const auto& cmd : validCommands) {
            std::cerr << "  - " << cmd.first << ": " << cmd.second << std::endl;
        }
        return std::nullopt;
    }

    return test;
}

int main(int argc, char* argv[]) {
    int min_memory = 0;
    int max_used_percent = 0;
    int max_temp = 0;
    int min_temp = 0;

    if (!LoadConfig(CONFIG_FILE, min_memory, max_used_percent, max_temp, min_temp)) {
        return 1;
    }

    std::cout << "MINIMUM_AVAILABLE_MEMORY: " << min_memory << std::endl;
    std::cout << "MAX_USED_PERCENT: " << max_used_percent << std::endl;
    std::cout << "MAX_TEMP: " << max_temp << std::endl;
    std::cout << "MIN_TEMP: " << min_temp << std::endl;

    auto test = parseCommandLineArguments(argc, argv);
    if (!test) {
        std::cout << "No valid test specified, performing all tests..." << std::endl;
        performAllTests();
    } else if (*test == "-h" || *test == "--help") {
        showHelp();
    } else {
        performTest(*test);
    }

    return 0;
}
