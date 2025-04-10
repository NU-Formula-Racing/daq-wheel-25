#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>
#include "define.h"

class WheelSpeed {
public:
    WheelSpeed();
    void update();
    float getMovingAverageRpm() const;
    static void countTickISR();

private:
    void countTick();
    float calculateMovingAverageRpm();

    volatile unsigned int tickCount;
    unsigned int tickBuffer[BUFFER_SIZE];
    unsigned int bufferIndex;
    unsigned long lastSampleTime;
    float movingAverageRpm;
    
    // Static instance pointer to allow the ISR to access this instance.
    static WheelSpeed* instance;
};

#endif
