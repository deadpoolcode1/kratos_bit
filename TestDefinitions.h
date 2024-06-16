#ifndef TESTDEFINITIONS_H
#define TESTDEFINITIONS_H

#include <json/json.h>
#include <string>

Json::Value createTestResult(const std::string &testName, const std::string &result);

Json::Value testI2C();
Json::Value testRTC();
Json::Value testGPIO();
Json::Value testIRQ();
Json::Value testUART();
Json::Value testSPI();
Json::Value testRGMII();
Json::Value testSPACE();
Json::Value testMEMORY();
Json::Value testFPGA();

#endif // TESTDEFINITIONS_H
