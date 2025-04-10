#include <Arduino.h>

// Constants
const int sensorPin = 2;                // pin connected to sensor output
volatile unsigned int tickCount = 0;    // Counts pulses from the sensor in the current 0.1s


void setup() {
    pinMode(sensorPin, INPUT);            // Set sensor pin as input
    attachInterrupt(digitalPinToInterrupt(sensorPin), countTick, FALLING); // Trigger on falling edge
    Serial.begin(9600);                   // Start serial communication for output
  }
  
void loop() {
    Serial.print('Tick count:');
    Serial.println(tickCount);
}

void countTick() {
    tickCount++;
  }