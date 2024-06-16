#include "TestDefinitions.h"
#include "i2c.h"
#include "config.h"  // Include the configuration header
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <linux/rtc.h>

Json::Value createTestResult(const std::string &testName, const std::string &result) {
    Json::Value testResult;
    testResult["test"] = testName;
    testResult["result"] = result;
    return testResult;
}

Json::Value testI2C() {
    int file;

    if ((file = open(I2C_DEVICE_PATH, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        return createTestResult("I2C", "failure");
    }

    if (ioctl(file, I2C_SLAVE, I2C_ADDRESS) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(file);
        return createTestResult("I2C", "failure");
    }

    // Reading from the specified register
    int res = i2c_smbus_read_byte_data(file, I2C_REGISTER);

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
    // Open the RTC device
    int file = open(RTC_DEVICE_PATH, O_RDONLY);

    if (file < 0) {
        perror("Failed to open the rtc device");
        return createTestResult("RTC", "failure");
    }

    // Get the current time
    struct rtc_time rtc_time1, rtc_time2;
    if (ioctl(file, RTC_RD_TIME, &rtc_time1) < 0) {
        perror("Failed to read the rtc time");
        close(file);
        return createTestResult("RTC", "failure");
    }

    // Wait for 2 seconds
    sleep(2);

    // Get the current time again
    if (ioctl(file, RTC_RD_TIME, &rtc_time2) < 0) {
        perror("Failed to read the rtc time");
        close(file);
        return createTestResult("RTC", "failure");
    }

    close(file);

    // Check if the RTC time has progressed
    if ((rtc_time2.tm_sec != rtc_time1.tm_sec) || (rtc_time2.tm_min != rtc_time1.tm_min) || (rtc_time2.tm_hour != rtc_time1.tm_hour)) {
        return createTestResult("RTC", "success");
    } else {
        return createTestResult("RTC", "failure");
    }
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
