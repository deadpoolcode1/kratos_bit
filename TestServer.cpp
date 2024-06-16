#include "TestServer.h"
#include "TestDefinitions.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/test_socket"

TestServer::TestServer() {
    // Initialize default CBIT time
    cbitTime = 10;
}

std::string TestServer::handleCommand(const std::string &command) {
    Json::Value root;
    Json::CharReaderBuilder readerBuilder;
    std::string errors;
    std::istringstream s(command);
    if (!Json::parseFromStream(readerBuilder, s, &root, &errors)) {
        return createErrorResponse("Invalid JSON");
    }

    std::string cmd = root["command"].asString();
    if (cmd == "READ_LATEST_RESULTS") {
        return handleReadLatestResults();
    } else if (cmd == "PERFORM_IBIT") {
        return handlePerformIBIT();
    } else if (cmd == "CHANGE_CBIT_TIME") {
        return handleChangeCBITTime(root);
    } else if (cmd == "READ_CBIT_TIME") {
        return handleReadCBITTime();
    } else {
        return createErrorResponse("Unknown command");
    }
}

std::string TestServer::handleReadLatestResults() {
    if (latestIbitResults.empty()) {
        return createErrorResponse("No IBIT results available");
    }
    return latestIbitResults;
}

std::string TestServer::handlePerformIBIT() {
    // Perform IBIT (stub)
    Json::Value response;
    response["status"] = "success";
    response["results"] = Json::arrayValue;
    response["results"].append(testI2C());
    response["results"].append(testRTC());
    response["results"].append(testGPIO());
    response["results"].append(testIRQ());
    response["results"].append(testUART());
    response["results"].append(testSPI());
    response["results"].append(testRGMII());
    response["results"].append(testSPACE());
    response["results"].append(testMEMORY());
    response["results"].append(testFPGA());
    response["timestamp"] = getCurrentTimestamp();
    latestIbitResults = response.toStyledString();
    return latestIbitResults;
}

std::string TestServer::handleChangeCBITTime(const Json::Value &root) {
    cbitTime = root["new_time"].asInt();
    Json::Value response;
    response["status"] = "success";
    response["new_time"] = cbitTime;
    return response.toStyledString();
}

std::string TestServer::handleReadCBITTime() {
    Json::Value response;
    response["status"] = "success";
    response["default_time"] = cbitTime;
    return response.toStyledString();
}

std::string TestServer::createErrorResponse(const std::string &message) {
    Json::Value response;
    response["status"] = "error";
    response["message"] = message;
    return response.toStyledString();
}

std::string TestServer::createSuccessResponse(const Json::Value &data) {
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, data);
}

Json::Value TestServer::performTest(const std::string &testName) {
    if (testName == "I2C") {
        return testI2C();
    } else if (testName == "RTC") {
        return testRTC();
    } else if (testName == "GPIO") {
        return testGPIO();
    } else if (testName == "IRQ") {
        return testIRQ();
    } else if (testName == "UART") {
        return testUART();
    } else if (testName == "SPI") {
        return testSPI();
    } else if (testName == "RGMII") {
        return testRGMII();
    } else if (testName == "SPACE") {
        return testSPACE();
    } else if (testName == "MEMORY") {
        return testMEMORY();
    } else if (testName == "FPGA") {
        return testFPGA();
    } else {
        return createTestResult(testName, "unknown test");
    }
}

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[sizeof "YYYY-MM-DDTHH:MM:SSZ"];
    std::strftime(buf, sizeof buf, "%FT%TZ", std::gmtime(&now));
    return buf;
}

void startServer() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buf[1024];
    TestServer server; // Single instance of TestServer

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket error");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind error");
        exit(-1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen error");
        exit(-1);
    }

    while (true) {
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("accept error");
            continue;
        }

        int n = read(client_fd, buf, sizeof(buf));
        if (n > 0) {
            buf[n] = '\0';
            std::string response = server.handleCommand(buf);
            write(client_fd, response.c_str(), response.size());
        }
        close(client_fd);
    }
}
