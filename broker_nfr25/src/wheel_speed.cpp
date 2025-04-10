#include "wheel_speed.h"

WheelSpeed::WheelSpeed(HWPin pin, uint16_t teethPerRevolution, uint32_t sampleIntervalMS) : _pin(pin), _tickCount(0), _bufferIndex(0), _lastSampleTime(0), _movingAverageRpm(0), _teethPerRevolution(teethPerRevolution), _sampleIntervalMS(sampleIntervalMS) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        _tickBuffer[i] = 0;
    }
}

void WheelSpeed::initalize() {
    pinMode(_pin, INPUT_PULLUP);
    attachInterruptArg(
        digitalPinToInterrupt(_pin),
        WheelSpeed::isrCountTick,
        (void *)this,
        FALLING);
}

float WheelSpeed::getRPM() {
    // unsigned long currentTime = millis();
    // if (currentTime - _lastSampleTime >= SAMPLE_INTERVAL) {
    //     noInterrupts();
    //     _tickBuffer[_bufferIndex] = _tickCount;
    //     _tickCount = 0;

    //     _bufferIndex = (_bufferIndex + 1) % BUFFER_SIZE;
    //     _lastSampleTime = currentTime;

    //     interrupts();

    //     _movingAverageRpm = calculateMovingAverageRpm();
    // }
}

float WheelSpeed::calculateMovingAverageRPM() {
    unsigned long totalTicks = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        totalTicks += _tickBuffer[i];
    }
    float avgTicksPerSample = totalTicks / (float)BUFFER_SIZE;
    float ticksPerSecond = avgTicksPerSample * (1000.0 / SAMPLE_INTERVAL);
    return (ticksPerSecond / TEETH_PER_REVOLUTION) * 60.0;
}

void WheelSpeed::isrCountTick(void *args) {
    WheelSpeed *speed = (WheelSpeed *)args;
    speed->_tickCount++;
}