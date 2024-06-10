#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <json/json.h>

#define SOCKET_PATH "/tmp/test_socket"

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
    testCommands();
    return 0;
}
