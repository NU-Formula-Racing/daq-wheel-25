#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>

#include "define.h"

/// @brief An abstration over any type of hall-effect based wheel speed sensor
class WheelSpeed {
   public:
    /// @brief Creates a wheel speed sensor
    /// @param pin The digital input pin
    /// @param teethPerRevolution The number of teeth per revolution
    /// @param sampleIntervalMS The rate at which the sensor is sampled
    WheelSpeed(HWPin pin, uint16_t teethPerRevolution, uint32_t sampleIntervalMS);

    /// @brief Initializes the sensor, by setting up the pin mode and attaching an interrupt
    void initalize();

    /// @brief Gets the current RPM of the wheel
    /// @return The RPM of the wheel
    float getRPM();

    private:
    /// @brief ISR Handler for when the pin goes from HIGH to LOW
    /// @param args A pointer to a WheelSpeed object
    static void isrCountTick(void *args);

    /// @brief Internal function for calculating a moving average for the speed
    /// @return The moving average
    float calculateMovingAverageRPM();
    
    // HW Varaibles
    HWPin _pin;

    // Bookkeeping variables
    volatile uint32_t _tickCount;
    uint32_t _tickBuffer[BUFFER_SIZE];
    uint32_t _bufferIndex;
    uint32_t _lastSampleTime;
    uint16_t _teethPerRevolution;
    uint32_t _sampleIntervalMS;
    float _movingAverageRpm;

};

#endif
