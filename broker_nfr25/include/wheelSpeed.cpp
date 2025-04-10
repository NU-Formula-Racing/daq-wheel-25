#include "wheelSpeed.h"

// Initialize the static instance pointer.
WheelSpeed* WheelSpeed::instance = nullptr;

WheelSpeed::WheelSpeed() : tickCount(0), bufferIndex(0), lastSampleTime(0), movingAverageRpm(0) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        tickBuffer[i] = 0;
    }
    instance = this;
    pinMode(WHEEL_SPEED_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WHEEL_SPEED_PIN), countTickISR, FALLING);
}

void WheelSpeed::countTick() {
    tickCount++;
}

// Static ISR callback that calls the instance method.
void WheelSpeed::countTickISR() {
    if (instance != nullptr) {
        instance->countTick();
    }
}

void WheelSpeed::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastSampleTime >= SAMPLE_INTERVAL) {
        noInterrupts();
        tickBuffer[bufferIndex] = tickCount;
        tickCount = 0;

        bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
        lastSampleTime = currentTime;
        
        interrupts();
        
        movingAverageRpm = calculateMovingAverageRpm();
    }
}

float WheelSpeed::calculateMovingAverageRpm() {
    unsigned long totalTicks = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        totalTicks += tickBuffer[i];
    }
    float avgTicksPerSample = totalTicks / (float)BUFFER_SIZE;
    float ticksPerSecond = avgTicksPerSample * (1000.0 / SAMPLE_INTERVAL);
    return (ticksPerSecond / TEETH_PER_REVOLUTION) * 60.0;
}

float WheelSpeed::getMovingAverageRpm() const {
    return movingAverageRpm;
}
