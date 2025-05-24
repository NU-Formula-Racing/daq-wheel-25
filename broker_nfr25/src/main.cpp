#include <Arduino.h>
#include <HardwareSerial.h>
#include "can_tx.h"
#include "define.h"
#include "sus_pot.h"
#include "wheel_speed.h"
#include "wheel_temp.h"

//Temp UART Pins
#define RXD1 18
#define TXD1 5

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
    case BrokerPosition::BP_FL:
         can.daq_wheel_front_left_outer_temps_flo_temperature_0 = temps[0];
         can.daq_wheel_front_left_outer_temps_flo_temperature_1 = temps[1];
         can.daq_wheel_front_left_outer_temps_flo_temperature_2 = temps[2];
         can.daq_wheel_front_left_outer_temps_flo_temperature_3 = temps[3];
         can.daq_wheel_front_left_inner_temps_fli_temperature_4 = temps[4];
         can.daq_wheel_front_left_inner_temps_fli_temperature_5 = temps[5];
         can.daq_wheel_front_left_inner_temps_fli_temperature_6 = temps[6];
         can.daq_wheel_front_left_inner_temps_fli_temperature_7 = temps[7];
        break;
    case BrokerPosition::BP_FR:
         can.daq_wheel_front_right_outer_temps_fro_temperature_0 = temps[0];
         can.daq_wheel_front_right_outer_temps_fro_temperature_1 = temps[1];
         can.daq_wheel_front_right_outer_temps_fro_temperature_2 = temps[2];
         can.daq_wheel_front_right_outer_temps_fro_temperature_3 = temps[3];
         can.daq_wheel_front_right_inner_temps_fri_temperature_4 = temps[4];
         can.daq_wheel_front_right_inner_temps_fri_temperature_5 = temps[5];
         can.daq_wheel_front_right_inner_temps_fri_temperature_6 = temps[6];
         can.daq_wheel_front_right_inner_temps_fri_temperature_7 = temps[7];
        break;
    case BrokerPosition::BP_BL:
         can.daq_wheel_back_left_outer_temps_blo_temperature_0 = temps[0];
         can.daq_wheel_back_left_outer_temps_blo_temperature_1 = temps[1];
         can.daq_wheel_back_left_outer_temps_blo_temperature_2 = temps[2];
         can.daq_wheel_back_left_outer_temps_blo_temperature_3 = temps[3];
         can.daq_wheel_back_left_inner_temps_bli_temperature_4 = temps[4];
         can.daq_wheel_back_left_inner_temps_bli_temperature_5 = temps[5];
         can.daq_wheel_back_left_inner_temps_bli_temperature_6 = temps[6];
         can.daq_wheel_back_left_inner_temps_bli_temperature_7 = temps[7];
        break;
    case BrokerPosition::BP_BR:
         can.daq_wheel_back_right_outer_temps_bro_temperature_0 = temps[0];
         can.daq_wheel_back_right_outer_temps_bro_temperature_1 = temps[1];
         can.daq_wheel_back_right_outer_temps_bro_temperature_2 = temps[2];
         can.daq_wheel_back_right_outer_temps_bro_temperature_3 = temps[3];
         can.daq_wheel_back_right_inner_temps_bri_temperature_4 = temps[4];
         can.daq_wheel_back_right_inner_temps_bri_temperature_5 = temps[5];
         can.daq_wheel_back_right_inner_temps_bri_temperature_6 = temps[6];
         can.daq_wheel_back_right_inner_temps_bri_temperature_7 = temps[7];
        break;
    }

    can.loadSignal = 0;
    can.tick();
}