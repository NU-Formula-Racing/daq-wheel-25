#ifndef DEFINE_H
#define DEFINE_H

#include <stdint.h>

enum HWPin : uint8_t {
    WHEEL_SPEED_PIN = GPIO_NUM_22,
    POT_PIN = GPIO_NUM_27
};

// Potentiometer Constants
#define POT_CONST (3.3 / 4095.0)
#define SLOPE 1380.0

// Wheel Speed Constants
#define TEETH_PER_REVOLUTION 70
#define SAMPLE_INTERVAL 100  // in milliseconds
#define BUFFER_SIZE 10

#endif
