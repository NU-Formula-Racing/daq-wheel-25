//basically all from NFR Learn

#include <Arduino.h>
//#include <Arduino_CAN.h>
#include <CAN.h>
#include <virtualTimer.h>

// put function declarations here:
CAN g_can{};
VirtualTimerGroup g_canTimerGroup;

MakeSignedCANSignal(float, 0, 16, 1.0, 0.0) g_wheelSpeedSignal;
MakeSignedCANSignal(float, 24, 16, 1.0, 0.0) g_susPotSignal;
MakeSignedCANSignal(uint8_t, 16, 8, 1.0, 0.0) g_wheelTempSignal;
MakeSignedCANSignal(uint8_t, 40, 8, 1.0, 0.0) g_strainGaugeSignal;

CANTXMessage<4> g_wheelMessage {
  g_can, // which CAN bus to use 
  0x123, // id of the message 
  8, // length of the message
  100, // period in ms
  // all the signals we want to send
  g_wheelSpeedSignal, g_susPotSignal, 
  g_wheelTempSignal, g_strainGaugeSignal
};

void setup() {
  // put your setup code here, to run once:
  g_can.Initialize(ICAN::BaudRate::kBaud1M);
  g_canTimerGroup.AddTimer(50, readSensors);
  g_canTimerGroup.AddTimer(10, tickCan);
}

void loop() {
  // put your main code here, to run repeatedly:
  g_canTimerGroup.Tick(millis());
}

void readSensors()
{
    // automatically updates our message with the new values
    g_wheelSpeedSignal = (float)random(0, 100);
    g_susPotSignal = (float)random(3, 52);
    g_wheelTempSignal = (uint8_t)random(12,25);
    g_strainGaugeSignal = (uint8_t)random(12,25);
}

void tickCan()
{
    g_can.Tick();
}