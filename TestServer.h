#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <string>
#include <json/json.h>
#include <thread>
#include <atomic>

class TestServer {
public:
    TestServer();
    ~TestServer();
    std::string handleCommand(const std::string &command);
    
private:
    std::string handleReadLatestResults();
    std::string handlePerformIBIT();
    std::string handleChangeCBITTime(const Json::Value &root);
    std::string handleReadCBITTime();
    std::string createErrorResponse(const std::string &message);
    std::string createSuccessResponse(const Json::Value &data);
    std::string performTest(const std::string &testName);
    std::string performPBIT();
    void runCBIT();
    void mergeCbitResults(const Json::Value &cbitResults);

    int cbitTime;
    std::string latestIbitResults;
    std::atomic<bool> cbitRunning;
    std::thread cbitThread;
};

std::string getCurrentTimestamp();
void startServer();

#endif // TESTSERVER_H
