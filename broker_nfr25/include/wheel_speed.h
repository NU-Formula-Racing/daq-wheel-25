#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>

#include "define.h"

class WheelSpeed {
   public:
    WheelSpeed(uint8_t pin, uint16_t teethPerRevolution, uint32_t sampleIntervalMS);

    void initalize();
    float getRPM();

    private:
    uint8_t _pin;
    static void isrCountTick(void *args);
    float calculateMovingAverageRPM();
    
    volatile uint32_t _tickCount;
    uint32_t _tickBuffer[BUFFER_SIZE];
    uint32_t _bufferIndex;
    uint32_t _lastSampleTime;
    uint16_t _teethPerRevolution;
    uint32_t _sampleIntervalMS;
    float _movingAverageRpm;

};

#endif
