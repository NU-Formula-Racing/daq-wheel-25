#ifndef DEFINE_H
#define DEFINE_H

#include <stdint.h>

#include "lut.h"

enum HWPin : uint8_t {
    WHEEL_SPEED_PIN = GPIO_NUM_22,
    POT_PIN = GPIO_NUM_27
};

// Potentiometer Constants
#define POT_CONST (3.3 / 4095.0)
#define SLOPE 1380.0

// LUT for SUS pot
static NumericLUT SUS_LUT = {
    {
        {0, 0},    // when we read 0 volts, we are at 0 cm
        {3.3, 3},  // when we read 3.3 volts, we are at 3 cm
    },
    InterpolationType::IT_LERP  // linear interpolation
};

// Wheel Speed Constants
#define TEETH_PER_REVOLUTION 70
#define SAMPLE_INTERVAL 100  // in milliseconds
#define BUFFER_SIZE 10

#endif
