#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include <json/json.h>

// I2C Device Path
const char I2C_DEVICE_PATH[] = "/dev/i2c-1";
const int I2C_ADDRESS = 0x72;

// RTC Device Path
const char RTC_DEVICE_PATH[] = "/dev/rtc0";

// GPIO Paths
const char GPIO_EXPORT_PATH[] = "/sys/class/gpio/export";
const char GPIO_PATH[] = "/sys/class/gpio/gpio0/value";

// UART Paths
const char UART0_PATH[] = "/dev/ttymxc0";
const char UART2_PATH[] = "/dev/ttymxc2";

// SPI Path
const char SPI_PATH[] = "/dev/spidev1.0";

// RGMII Interfaces
const char RGMII_INTERFACE1[] = "eth0";
const char RGMII_INTERFACE2[] = "eth1";

// IRQ Path
const char IRQ_PATH[] = "/proc/interrupts";

// Common Constants
const int I2C_REGISTER = 0x00;

extern int MINIMUM_AVAILABLE_MEMORY;
extern int MAX_USED_PERCENT;
extern int MAX_TEMP;
extern int MIN_TEMP;

// Function to load configuration
bool LoadConfig(const std::string& filename, int& min_memory, int& max_used_percent, int& max_temp, int& min_temp);

#endif // CONFIG_H
