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

// IRQ Path
const char IRQ_PATH[] = "/proc/interrupts";

// Common Macros or Constants
const int I2C_REGISTER = 0x00;

#endif // CONFIG_H
