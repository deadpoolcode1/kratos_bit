#include "TestServer.h"
#include "TestDefinitions.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <json/json.h>
#include <thread>
#include <chrono>

#define SOCKET_PATH "/tmp/uds_socket"

TestServer::TestServer() : cbitTime(10), cbitRunning(true), cbitThread(&TestServer::runCBIT, this) {
    // Perform PBIT on startup and store the results
    latestPbitResults.results = Json::arrayValue;
    latestPbitResults.timestamp = getCurrentTimestamp();
    latestPbitResults.results.append(testSPACE());
    latestPbitResults.results.append(testTemperature());
}

TestServer::~TestServer() {
    cbitRunning = false;
    if (cbitThread.joinable()) {
        cbitThread.join();
    }
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
    } else if (cmd == "READ_PBIT") {
        return handleReadPBIT();
    } else if (cmd == "PERFORM_TEST_TEMP") {
        return performTest("TEMP");
    } else if (cmd == "PERFORM_TEST_FREESPACE") {
        return performTest("FREESPACE");
    } else if (cmd == "PERFORM_TEST_I2C") {
        return performTest("I2C");
    } else if (cmd == "PERFORM_TEST_RTC") {
        return performTest("RTC");
    } else if (cmd == "PERFORM_TEST_GPIO") {
        return performTest("GPIO");
    } else if (cmd == "PERFORM_TEST_IRQ") {
        return performTest("IRQ");
    } else if (cmd == "PERFORM_TEST_UART") {
        return performTest("UART");
    } else if (cmd == "PERFORM_TEST_SPI") {
        return performTest("SPI");
    } else if (cmd == "PERFORM_TEST_RGMII") {
        return performTest("RGMII");
    } else if (cmd == "PERFORM_TEST_MEMORY") {
        return performTest("MEMORY");
    } else if (cmd == "PERFORM_TEST_FPGA") {
        return performTest("FPGA");
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
    // Perform IBIT and store the results
    latestIbitResults = performPBIT(); // Use the same function for IBIT and PBIT
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

std::string TestServer::handleReadPBIT() {
    Json::Value response;
    response["status"] = "success";
    response["results"] = latestPbitResults.results;
    response["timestamp"] = latestPbitResults.timestamp;
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

std::string TestServer::performTest(const std::string &testName) {
    Json::Value response;
    response["status"] = "success";
    response["results"] = Json::arrayValue;

    if (testName == "FREESPACE") {
        response["results"].append(testSPACE());
    } else if (testName == "TEMP") {
        response["results"].append(testTemperature());
    } else if (testName == "I2C") {
        response["results"].append(testI2C());
    } else if (testName == "RTC") {
        response["results"].append(testRTC());
    } else if (testName == "GPIO") {
        response["results"].append(testGPIO());
    } else if (testName == "IRQ") {
        response["results"].append(testIRQ());
    } else if (testName == "UART") {
        response["results"].append(testUART());
    } else if (testName == "SPI") {
        response["results"].append(testSPI());
    } else if (testName == "RGMII") {
        response["results"].append(testRGMII());
    } else if (testName == "MEMORY") {
        response["results"].append(testMEMORY());
    } else if (testName == "FPGA") {
        response["results"].append(testFPGA());
    } else {
        return createErrorResponse("Unknown test");
    }
    
    return response.toStyledString();
}

std::string TestServer::performPBIT() {
    // Perform PBIT (stub)
    Json::Value response;
    response["status"] = "success";
    response["results"] = Json::arrayValue;
    response["results"].append(testSPACE());
    response["results"].append(testTemperature());
    response["timestamp"] = getCurrentTimestamp();
    return response.toStyledString();
}

void TestServer::runCBIT() {
    bool ibitDone = false;

    while (cbitRunning) {
        // Perform IBIT once if it hasn't been done
        if (!ibitDone) {
            std::cout << "No IBIT results available. Performing initial IBIT..." << std::endl;
            latestIbitResults = performPBIT();
            std::cout << "Initial IBIT completed." << std::endl;
            ibitDone = true;
        }

        // Perform CBIT tests (testSPACE and testMEMORY)
        Json::Value cbitResults;
        cbitResults["status"] = "success";
        cbitResults["results"] = Json::arrayValue;
        cbitResults["results"].append(testSPACE());
        cbitResults["results"].append(testTemperature());
        cbitResults["timestamp"] = getCurrentTimestamp();

        // Log each CBIT run
        std::cout << "CBIT run at: " << cbitResults["timestamp"].asString() << std::endl;

        // Merge the latest CBIT results with the IBIT results
        mergeCbitResults(cbitResults);

        // Simulate periodic CBIT execution
        std::this_thread::sleep_for(std::chrono::seconds(cbitTime));
    }
}


void TestServer::mergeCbitResults(const Json::Value &cbitResults) {
    Json::CharReaderBuilder readerBuilder;
    Json::Value ibitResults;
    std::string errors;

    std::istringstream s(latestIbitResults);
    if (!Json::parseFromStream(readerBuilder, s, &ibitResults, &errors)) {
        std::cerr << "Failed to parse IBIT results: " << errors << std::endl;
        return;
    }

    for (const auto &cbitResult : cbitResults["results"]) {
        std::string testName = cbitResult["test"].asString();
        bool found = false;

        for (auto &ibitResult : ibitResults["results"]) {
            if (ibitResult["test"].asString() == testName) {
                ibitResult = cbitResult;
                found = true;
                break;
            }
        }

        if (!found) {
            ibitResults["results"].append(cbitResult);
        }
    }

    ibitResults["timestamp"] = cbitResults["timestamp"];
    latestIbitResults = ibitResults.toStyledString();
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
