#ifndef DEFINE_H
#define DEFINE_H

#include <stdint.h>

#include "lut.h"

// Resistor Configuration -> Wheel position specification
// 
// When indicator is high, it is the FRONT or LEFT wheel
//
// _1 -> Left or Right
// _2 -> Front or Back
//
// R7 -> Pullup to 3v3 () -> Left or Front
// R8 -> Pulldown to GND -> Back or Right
//
// R7_1 & R7_2 -> Front Left (Luscious Locks)
// R8_1 & R7_2 -> Front Right
// R7_1 & R8_2 -> Back Left 
// R8_1 & R8_2 -> Back Right


enum HWPin : uint8_t {
    WHEEL_SPEED_PIN = GPIO_NUM_22,
    POT_PIN = GPIO_NUM_15,
    POWER_LED = GPIO_NUM_25,
    TEMP_POWER_LED = GPIO_NUM_26,
    FB_SETTING = GPIO_NUM_34,
    FB_INDICATOR = GPIO_NUM_35,
    LR_SETTING = GPIO_NUM_32,
    LR_INDICATOR = GPIO_NUM_33,
    TEMP_TX = GPIO_NUM_18,
    TEMP_RX = GPIO_NUM_5,
};

// Potentiometer Constants
#define POT_CONST (3.3 / 4095.0)
#define SLOPE 1380.0

// LUT for SUS pot
static NumericLUT SUS_LUT = {
    {
        {0, 0},    // when we read 0 volts, we are at 0 cm
        {3.3, 25},  // when we read 3.3 volts, we are at 25 cm
    },
    InterpolationType::IT_LERP  // linear interpolation
};

// Wheel Speed Constants
#define TEETH_PER_REVOLUTION 70
#define SAMPLE_INTERVAL 100  // in milliseconds
#define BUFFER_SIZE 10

#endif
