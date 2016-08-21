/*
 * Pano Controller Master Configuration File
 */

#if defined(__AVR__)
#error "AVR is not supported"

#elif !defined(ARDUINO_SAMD_FEATHER_M0)
#include "config_teensy.h"

#else
#include "config_feather_m0.h"
#endif

// Address of I2C OLED display. If screen looks scaled edit Adafruit_SSD1306.h
// and pick SSD1306_128_64 or SSD1306_128_32 that matches display type.
#define DISPLAY_I2C_ADDRESS 0x3C
#define OLED_RESET 12
#define TEXT_SIZE 1
#define DISPLAY_ROWS SSD1306_LCDHEIGHT/8/TEXT_SIZE
#define DISPLAY_COLS SSD1306_LCDWIDTH/6/TEXT_SIZE

// Battery monitoring settings
#define VCC 3300
#define LOW_BATTERY 7000
// R1/R2 is the voltage divisor in Ω (GND-R1-A0-R2-Vin)
// measure resistors and enter actual values for a more accurate voltage
#define BATT_R1 9980
#define BATT_R2 46500
#define BATT_RANGE (VCC * (BATT_R1 + BATT_R2) / BATT_R1)

// MPU (accel/gyro)
#define MPU_I2C_ADDRESS 0x68

// Stepper motors steps per revolution
#define MOTOR_STEPS 200
