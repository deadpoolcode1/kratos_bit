
#ifndef CONFIG_H
#define CONFIG_H

// I2C Device Path
const char I2C_DEVICE_PATH[] = "/dev/i2c-1";
const int I2C_ADDRESS = 0x72;

// RTC Device Path
const char RTC_DEVICE_PATH[] = "/dev/rtc0";

// Common Macros or Constants
const int I2C_REGISTER = 0x00;

#endif // CONFIG_H
