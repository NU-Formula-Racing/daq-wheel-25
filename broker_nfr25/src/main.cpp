#include <Arduino.h>
#include <HardwareSerial.h>
#include "can_tx.h"
#include "define.h"
#include "sus_pot.h"
#include "wheel_speed.h"
#include "wheel_temp.h"

CANTX can;
HardwareSerial tempSerial(1); // Temp board serial

WheelSpeed wheelSpeed{
    HWPin::WHEEL_SPEED_PIN,
    TEETH_PER_REVOLUTION,
    SAMPLE_INTERVAL};

SusPot susPot{
    HWPin::POT_PIN,
    10000,
    SUS_LUT};

WheelTemp wheelTemp{
    tempSerial};

void setup()
{
    tempSerial.begin(115200, SERIAL_8N1, RXD1, TXD1); // Temp board recieving serial lines
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

void loop()
{
    float currentRpm = wheelSpeed.getRPM();
    can.wheelSpeedSignal = currentRpm;

    float currentDisplacement = susPot.getDisplacement();
    can.displacementSignal = currentDisplacement;

    std::array<float, 8> temps = wheelTemp.getTemps();
    switch (can.position)
    {
    case BrokerPosition::BP_FL
        break;
    case BrokerPosition::BP_FR:
        break;
    case BrokerPosition::BP_BL:
        break;
    case BrokerPosition::BP_BR:
        break;
    }

    can.loadSignal = 0;
    can.tick();
}