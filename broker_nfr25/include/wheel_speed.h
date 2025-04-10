#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>

#include "define.h"

struct __WheelTickInfo {
    uint64_t tickTime; // the last time that the entry was updated
    uint64_t sampleInterval; // the amount of time in ms that this tick info covers
    uint32_t tickValue; // the value
    bool valid; // does this actually mean something?
};

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

    /// @brief Updates the reading of the sensor. Technically you don't have to call this,
    /// but doing so will make your measurement more accurate
    void update();

    /// @brief Gets the current RPM of the wheel
    /// @return The RPM of the wheel
    float getRPM();

    private:
    /// @brief ISR Handler for when the pin goes from HIGH to LOW
    /// @param args A pointer to a WheelSpeed object
    static void isrCountTick(void *args);

    /// @brief Internal function for calculating a moving average for the speed
    /// @return The moving average
    float calcualtedWeightedMovingAverageRPM();
    
    // HW Varaibles
    HWPin _pin;

    // Bookkeeping variables
    volatile uint32_t _tickCount;
    __WheelTickInfo _tickBuffer[BUFFER_SIZE];

    uint32_t _bufferIndex;
    uint64_t _lastSampleTime;
    uint16_t _teethPerRevolution;
    uint32_t _sampleIntervalMS;
    float _cachedMovingAverageRPM;

};

#endif
