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

SusPot susPot{
    HWPin::POT_PIN,
    10000,
    SUS_LUT};

void setup() {
    Serial.begin(115200);
    // turn on the power indicator
    Serial.println("Initializing Power LED!");
    pinMode(HWPin::POWER_LED, OUTPUT);
    digitalWrite(HWPin::POWER_LED, HIGH);

    Serial.println("Initializing Wheel Speed!");
    wheelSpeed.initalize();
    susPot.initialize();
    Serial.println("Initializing CAN!");
    can.initialize();
}

void loop() {
    // float currentDisplacement = susPot.getDisplacement();
    // can.displacementSignal = currentDisplacement;
    // Update the wheel speed calculations.;
    float currentRpm = wheelSpeed.getRPM();
    can.wheelSpeedSignal = currentRpm;
    float currentDisplacement = susPot.getDisplacement();
    can.displacementSignal = currentDisplacement;
    can.loadSignal = 0;

    can.tick();
}