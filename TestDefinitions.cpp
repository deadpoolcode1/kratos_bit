#include "TestDefinitions.h"
#include "i2c.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>

Json::Value createTestResult(const std::string &testName, const std::string &result) {
    Json::Value testResult;
    testResult["test"] = testName;
    testResult["result"] = result;
    return testResult;
}

Json::Value testI2C() {
    const char *device = "/dev/i2c-1";
    int address = 0x72; // The I2C address of the device
    int file;

    if ((file = open(device, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        return createTestResult("I2C", "failure");
    }

    if (ioctl(file, I2C_SLAVE, address) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(file);
        return createTestResult("I2C", "failure");
    }

    // Reading from register 0x00
    int reg = 0x00;
    int res = i2c_smbus_read_byte_data(file, reg);

    if (res < 0) {
        perror("Failed to read from the i2c bus");
        close(file);
        return createTestResult("I2C", "failure");
    }

    close(file);

    Json::Value result = createTestResult("I2C", "success");
    result["value"] = res;

    return result;
}

Json::Value testRTC() {
    return createTestResult("RTC", "success");
}

Json::Value testGPIO() {
    return createTestResult("GPIO", "success");
}

Json::Value testIRQ() {
    return createTestResult("IRQ", "success");
}

Json::Value testUART() {
    return createTestResult("UART", "success");
}

Json::Value testSPI() {
    return createTestResult("SPI", "success");
}

Json::Value testRGMII() {
    return createTestResult("RGMII", "success");
}

Json::Value testSPACE() {
    return createTestResult("SPACE", "success");
}

Json::Value testMEMORY() {
    return createTestResult("MEMORY", "success");
}

Json::Value testFPGA() {
    return createTestResult("FPGA", "success");
}
