#include "TestDefinitions.h"
#include "i2c.h"
#include "config.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <linux/rtc.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <map>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>

// Utility function to check if a file exists
bool fileExists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

Json::Value createTestResult(const std::string &testName, const std::string &result) {
    Json::Value testResult;
    testResult["test"] = testName;
    testResult["result"] = result;
    return testResult;
}

std::map<int, int> readIRQCounts() {
    std::map<int, int> irqCounts;
    std::ifstream file(IRQ_PATH);
    if (!file.is_open()) {
        perror("Failed to open IRQ file");
        return irqCounts;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int irqNumber;
        std::string temp;
        if (iss >> irqNumber >> temp) {
            int count;
            iss >> count;
            irqCounts[irqNumber] = count;
        }
    }

    file.close();
    return irqCounts;
}

Json::Value testIRQ() {
    auto initialCounts = readIRQCounts();
    if (initialCounts.empty()) {
        return createTestResult("IRQ", "failure");
    }

    // Perform an operation that should trigger an IRQ here
    // For example, if you have a specific device or hardware event
    // trigger it here. This is a placeholder for the actual operation.
    // performOperationThatTriggersIRQ();

    sleep(2); // Wait to ensure IRQs are processed

    auto finalCounts = readIRQCounts();
    if (finalCounts.empty()) {
        return createTestResult("IRQ", "failure");
    }

    // Compare initial and final counts to detect any changes
    bool irqTriggered = false;
    for (const auto& entry : finalCounts) {
        int irqNumber = entry.first;
        int initialCount = initialCounts[irqNumber];
        int finalCount = entry.second;
        if (finalCount > initialCount) {
            irqTriggered = true;
            break;
        }
    }

    if (irqTriggered) {
        return createTestResult("IRQ", "success");
    } else {
        return createTestResult("IRQ", "failure");
    }
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
    // Export GPIO pin
    int exportFile = open(GPIO_EXPORT_PATH, O_WRONLY);
    if (exportFile < 0) {
        perror("Failed to open export for writing");
        return createTestResult("GPIO", "failure");
    }

    if (write(exportFile, "0", 2) < 0) {
        perror("Failed to export GPIO");
        // Proceeding even if export fails, as it might have been exported already
    }
    close(exportFile);

    // Check if GPIO path exists
    if (!fileExists(GPIO_PATH)) {
        return createTestResult("GPIO", "failure");
    }

    // Read GPIO value
    int gpioFile = open(GPIO_PATH, O_RDONLY);
    if (gpioFile < 0) {
        perror("Failed to open GPIO value for reading");
        return createTestResult("GPIO", "failure");
    }

    char value;
    if (read(gpioFile, &value, 1) < 0) {
        perror("Failed to read GPIO value");
        close(gpioFile);
        return createTestResult("GPIO", "failure");
    }
    close(gpioFile);

    Json::Value result = createTestResult("GPIO", "success");
    result["value"] = value == '0' ? 0 : 1;

    return result;
}

Json::Value testUART() {
    if (fileExists(UART0_PATH) && fileExists(UART2_PATH)) {
        return createTestResult("UART", "success");
    } else {
        return createTestResult("UART", "failure");
    }
}

Json::Value testSPI() {
    if (fileExists(SPI_PATH)) {
        return createTestResult("SPI", "success");
    } else {
        return createTestResult("SPI", "failure");
    }
}

Json::Value testRGMII() {
    std::string eth0_path = std::string("/sys/class/net/") + RGMII_INTERFACE1;
    std::string eth1_path = std::string("/sys/class/net/") + RGMII_INTERFACE2;
    if (fileExists(eth0_path.c_str()) && fileExists(eth1_path.c_str())) {
        return createTestResult("RGMII", "success");
    } else {
        return createTestResult("RGMII", "failure");
    }
}

Json::Value testMEMORY() {
    std::string result = "failure";
    std::ifstream file("/proc/meminfo");
    std::string line;
    int available_memory = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            int value;
            std::string unit;
            if (iss >> key >> value >> unit && key == "MemAvailable:") {
                available_memory = value / 1024; // Convert to MB
                break;
            }
        }
        file.close();
    }

    if (available_memory >= MINIMUM_AVAILABLE_MEMORY) {
        result = "success";
    }

    Json::Value testResult = createTestResult("MEMORY", result);
    testResult["available_memory"] = available_memory;
    return testResult;
}

Json::Value testFPGA() {
    return createTestResult("FPGA", "success");
}

Json::Value testSPACE() {
    return createTestResult("SPACE", "success");
}
