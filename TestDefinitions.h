#ifndef TESTDEFINITIONS_H
#define TESTDEFINITIONS_H

#define TEMP_PATH "/sys/devices/virtual/thermal/thermal_zone0/temp"

#include <json/json.h>
#include <string>
#include <sys/statvfs.h>
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
Json::Value testTemperature();
#endif // TESTDEFINITIONS_H
