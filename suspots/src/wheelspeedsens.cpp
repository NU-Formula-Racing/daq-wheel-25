#include <Arduino.h>

// Constants
const int sensorPin = 2;                // pin connected to sensor output
const int teethPerRevolution = 10;      // teeth per revolution
const int sampleInterval = 100;         // interval in ms to check ticks 
const int bufferSize = 10;              // number of intervals in moving average

// Variables
volatile unsigned int tickCount = 0;    // Counts pulses from the sensor in the current 0.1s
unsigned int tickBuffer[bufferSize] = {0}; // Circular buffer for storing tick counts
unsigned int bufferIndex = 0;           // Current index in the buffer
unsigned long lastSampleTime = 0;       // Last time we sampled the ticks
float movingAverageRPM = 0;             // Moving average RPM

void setup() {
  pinMode(sensorPin, INPUT);            // Set sensor pin as input
  attachInterrupt(digitalPinToInterrupt(sensorPin), countTick, FALLING); // Trigger on falling edge
  Serial.begin(9600);                   // Start serial communication for output
}

void loop() {
  unsigned long currentMillis = millis();

  // Every 0.1 seconds, update the buffer and calculate the moving average
  if (currentMillis - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentMillis;

    // Store tick count in the circular buffer
    tickBuffer[bufferIndex] = tickCount;
    bufferIndex = (bufferIndex + 1) % bufferSize; // Move to the next index (circularly)

    // Reset tick count for the next interval
    tickCount = 0;
    
    // Print the moving average RPM
    Serial.print("Moving Average RPM: ");
    Serial.println(calculateMovingAverageRPM());
  }
}

// Interrupt Service Routine (ISR) for counting ticks
void countTick() {
  tickCount++;
}

// Function to calculate moving average RPM
float calculateMovingAverageRPM() {
  unsigned int totalTicks = 0;
  unsigned int samples = 0;

  // Sum up ticks from the buffer (handling case where time < 1 second)
  for (int i = 0; i < bufferSize; i++) {
    totalTicks += tickBuffer[i];
    if (tickBuffer[i] > 0) samples++;  // Count valid samples
  }

  // If we have samples, calculate the RPM
  if (samples > 0) {
    float ticksPerSecond = (totalTicks / (float)samples) * (1000.0 / sampleInterval);
    return (ticksPerSecond / teethPerRevolution) * 60.0;
  } else {
    return 0.0; // Return 0 if no valid samples
  }
}