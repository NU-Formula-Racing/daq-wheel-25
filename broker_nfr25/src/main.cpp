#include <Arduino.h>

#include "define.h"
#include "wheel_speed.h"
#include "sus_pot.h"

// Create an instance of WheelSpeed.
WheelSpeed wheelSpeed {
    HWPin::WHEEL_SPEED_PIN,
        TEETH_PER_REVOLUTION,
        SAMPLE_INTERVAL
};

SusPot susPot {
    HWPin::POT_PIN,
        10000,
        SUS_LUT
};



void setup() {
    Serial.begin(115200);
    // Configure the potentiometer pin.
    pinMode(POT_PIN, INPUT);
    wheelSpeed.initalize();

}

void loop() {
    // Read the potentiometer value.
    int potValue = analogRead(POT_PIN);
    float voltage = POT_CONST * potValue;
    float distance = potValue / SLOPE;

    Serial.print("Potentiometer Value: ");
    Serial.println(potValue);
    Serial.print("Voltage: ");
    Serial.println(voltage);
    Serial.print("Distance: ");
    Serial.println(distance);

    // Update the wheel speed calculations.;
    float currentRpm = wheelSpeed.getRPM();
    Serial.print("Moving Average RPM: ");
    Serial.println(currentRpm);

    // Short delay to allow the Serial Monitor to update at a readable rate.
    delay(100);
}