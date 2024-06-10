#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <string>
#include <unordered_map>
#include <json/json.h>

class TestServer {
public:
    TestServer();
    std::string handleCommand(const std::string &command);

private:
    int cbitTime;
    std::string latestIbitResults;

    std::string handleReadLatestResults();
    std::string handlePerformIBIT();
    std::string handleChangeCBITTime(const Json::Value &root);
    std::string handleReadCBITTime();

    std::string createErrorResponse(const std::string &message);
    std::string createSuccessResponse(const Json::Value &data);
    Json::Value performTest(const std::string &testName);
};

std::string getCurrentTimestamp();
void startServer();

#endif // TESTSERVER_H
