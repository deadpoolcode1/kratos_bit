// TestClient.cpp
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <json/json.h>
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

void testCommands() {
    Json::Value root;
    root["command"] = "READ_LATEST_RESULTS";
    std::string command = Json::writeString(Json::StreamWriterBuilder(), root);
    std::cout << "Sending: " << command << std::endl;
    std::cout << "Response: " << sendCommand(command) << std::endl;

    root["command"] = "PERFORM_IBIT";
    command = Json::writeString(Json::StreamWriterBuilder(), root);
    std::cout << "Sending: " << command << std::endl;
    std::cout << "Response: " << sendCommand(command) << std::endl;

    root["command"] = "CHANGE_CBIT_TIME";
    root["new_time"] = 20;
    command = Json::writeString(Json::StreamWriterBuilder(), root);
    std::cout << "Sending: " << command << std::endl;
    std::cout << "Response: " << sendCommand(command) << std::endl;

    root["command"] = "READ_CBIT_TIME";
    command = Json::writeString(Json::StreamWriterBuilder(), root);
    std::cout << "Sending: " << command << std::endl;
    std::cout << "Response: " << sendCommand(command) << std::endl;
}

int main() {
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

    testCommands();
    return 0;
}
