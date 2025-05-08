#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <Arduino.h>

#include "lut.hpp"

enum HWPin {
    IR1_THERMISTOR = GPIO_NUM_0,
    IR1_THERMOPILE = GPIO_NUM_4,
    IR2_THERMISTOR = GPIO_NUM_2,
    IR2_THERMOPILE = GPIO_NUM_15,
    IR3_THERMISTOR = GPIO_NUM_12,
    IR3_THERMOPILE = GPIO_NUM_13,
    IR4_THERMISTOR = GPIO_NUM_14,
    IR4_THERMOPILE = GPIO_NUM_27,
    IR5_THERMISTOR = GPIO_NUM_26,
    IR5_THERMOPILE = GPIO_NUM_25,
    IR6_THERMISTOR = GPIO_NUM_33,
    IR6_THERMOPILE = GPIO_NUM_32,
    IR7_THERMISTOR = GPIO_NUM_35,
    IR7_THERMOPILE = GPIO_NUM_34,
    IR8_THERMISTOR = GPIO_NUM_39,
    IR8_THERMOPILE = GPIO_NUM_36,
    NONE = GPIO_NUM_0
};

#define NUM_SENSORS 8
#define THERMOPILE_GAIN 246
#define THERMOPILE_OFFSET 0.55

static HWPin getThermopilePin(int thermopilePin) {
    static HWPin _thermopileLUT[NUM_SENSORS] = {
        IR1_THERMOPILE,
        IR2_THERMOPILE,
        IR3_THERMOPILE,
        IR4_THERMOPILE,
        IR5_THERMOPILE,
        IR6_THERMOPILE,
        IR7_THERMOPILE,
        IR8_THERMOPILE,
    };

    return _thermopileLUT[thermopilePin];
}

static HWPin getThermistorPin(int thermistorPin) {
    static HWPin _thermistorLUT[NUM_SENSORS] = {
        IR2_THERMISTOR,  // we had to cut the trace, use IR2
        IR2_THERMISTOR,
        IR3_THERMISTOR,
        IR4_THERMISTOR,
        IR5_THERMISTOR,
        IR6_THERMISTOR,
        IR7_THERMISTOR,
        IR8_THERMISTOR,
    };

    return _thermistorLUT[thermistorPin];
}

static NumericLUT __thermopileLUT = { // V -> C
    {
        {-0.002, 0},
        {-0.001, 10},
        {-0.0025, 20},
        {0.0005, 30},
        {0.0013, 40},
        {0.00225, 50},
        {0.0033, 60},
        {0.0045, 70},
        {0.006, 80},
        {0.00725, 90},
        {0.00975, 100},
    },
    InterpolationType::IT_LERP};

#endif  // __DEFINE_H__