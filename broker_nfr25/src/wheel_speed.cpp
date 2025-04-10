#include "wheel_speed.h"

WheelSpeed::WheelSpeed(HWPin pin, uint16_t teethPerRevolution, uint32_t sampleIntervalMS) : _pin(pin), _tickCount(0), _bufferIndex(0), _lastSampleTime(0), _cachedMovingAverageRPM(0), _teethPerRevolution(teethPerRevolution), _sampleIntervalMS(sampleIntervalMS) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        _tickBuffer[i] = (__WheelTickInfo){
            .tickTime = 0,
            .sampleInterval = 0,
            .tickValue = 0,
            .valid = false};
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

void WheelSpeed::update() {
    getRPM();  // perform the logic to update the buffers
}

float WheelSpeed::getRPM() {
    uint64_t currentTime = millis();
    // has enough time passed where we can sample the sensor?
    // if not, return our cached value
    if (currentTime - _lastSampleTime <= _sampleIntervalMS)
        return _cachedMovingAverageRPM;

    // store this in the buffer
    noInterrupts();
    _tickBuffer[_bufferIndex] = (__WheelTickInfo){
        .tickTime = currentTime,
        .sampleInterval = currentTime - _lastSampleTime,
        .tickValue = _tickCount,
        .valid = true};
    _lastSampleTime = currentTime;
    _tickCount = 0;
    _bufferIndex = (_bufferIndex + 1) % BUFFER_SIZE;
    interrupts();

    _cachedMovingAverageRPM = calcualtedWeightedMovingAverageRPM();
    return _cachedMovingAverageRPM;
}

float WheelSpeed::calcualtedWeightedMovingAverageRPM() {
    float totalWeightedTicksPerMS = 0;
    float totalWeight = 0;
    uint64_t currentTime = millis();

    for (int i = 0; i < BUFFER_SIZE; i++) {
        __WheelTickInfo info = _tickBuffer[i];
        if (info.valid == false) continue;  // this value is garbage
        float ticksPerMS = (float)info.tickValue / (float)info.sampleInterval;

        // now calculate a weight
        uint64_t timeSince = currentTime - info.tickTime;
        float weight = 1 / (float)timeSince;

        totalWeight += weight;
        totalWeightedTicksPerMS += weight * ticksPerMS;
    }

    float weightedAverageTicksPerMS = totalWeightedTicksPerMS / totalWeight;

    // convert ticks/ms -> ticks/sec -> rot/min
    float ticksPerSecond = weightedAverageTicksPerMS * (1000.0 / SAMPLE_INTERVAL);
    return (ticksPerSecond / TEETH_PER_REVOLUTION) * 60.0;
}

void WheelSpeed::isrCountTick(void *args) {
    WheelSpeed *speed = (WheelSpeed *)args;
    speed->_tickCount++;
}