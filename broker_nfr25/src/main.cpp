#include <Arduino.h>

#include "can_tx.h"
#include "define.h"
#include "sus_pot.h"
#include "wheel_speed.h"

CANTX can;

// Create an instance of WheelSpeed.
WheelSpeed wheelSpeed{
    HWPin::WHEEL_SPEED_PIN,
    TEETH_PER_REVOLUTION,
    SAMPLE_INTERVAL};

// SusPot susPot{
//     HWPin::POT_PIN,
//     10000,
//     SUS_LUT};

void setup() {
    Serial.begin(115200);
    // Configure the potentiometer pin.

    wheelSpeed.initalize();
    // susPot.initialize();

    can.initialize();
}

void loop() {
    // float currentDisplacement = susPot.getDisplacement();
    // can.displacementSignal = currentDisplacement;

    // Update the wheel speed calculations.;
    float currentRpm = wheelSpeed.getRPM();
    can.wheelSpeedSignal = currentRpm;

    can.tick();
}