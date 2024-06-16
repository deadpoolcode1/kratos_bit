#ifndef CONFIG_H
#define CONFIG_H

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
const int MINIMUM_AVAILABLE_MEMORY = 100; // Minimum available memory in MB
const int MAX_USED_PERCENT = 90; // Maximum allowed used percent for root filesystem

#endif // CONFIG_H
