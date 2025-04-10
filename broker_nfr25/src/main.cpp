#include <Arduino.h>
#include "define.h"
#include "wheelSpeed.h"

// Create an instance of WheelSpeed.
WheelSpeed wheelSpeed;

void setup() {
    Serial.begin(115200);
    // Configure the potentiometer pin.
    pinMode(POT_PIN, INPUT);
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
    
    // Update the wheel speed calculations.
    wheelSpeed.update();
    float currentRpm = wheelSpeed.getMovingAverageRpm();
    Serial.print("Moving Average RPM: ");
    Serial.println(currentRpm);
    
    // Short delay to allow the Serial Monitor to update at a readable rate.
    delay(100);
}




//Code below idk what its for or if it works

// #include <Arduino.h>

// // not sure if this is the right code for wheel speed sensor. will test later


// // Constants
// const int speed_sensor_pin = 22;         // pin connected to sensor output
// const int teethPerRevolution = 10;      // teeth per revolution
// const int sampleInterval = 100;         // interval in ms to check ticks 
// const int bufferSize = 10;              // number of intervals in moving average

// // Variables
// volatile unsigned int tickCount = 0;    // Counts pulses from the sensor in the current 0.1s
// unsigned int tickBuffer[bufferSize] = {0}; // Circular buffer for storing tick counts
// unsigned int bufferIndex = 0;           // Current index in the buffer
// unsigned long lastSampleTime = 0;       // Last time we sampled the ticks
// float movingAverageRPM = 0;             // Moving average RPM

// // Define TX and RX pins for UART (change if needed)
// #define TXD1 19
// #define RXD1 21

// // Use Serial1 for UART communication
// HardwareSerial mySerial(2);

// void setup() {
//   pinMode(speed_sensor_pin, INPUT);            // Set sensor pin as input
//   attachInterrupt(digitalPinToInterrupt(speed_sensor_pin), countTick, FALLING); // Trigger on falling edge
//   mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART setup
// }

// void loop() {
//   unsigned long currentMillis = millis();

//   // Every 0.1 seconds, update the buffer and calculate the moving average
//   if (currentMillis - lastSampleTime >= sampleInterval) {
//     lastSampleTime = currentMillis;

//     // Store tick count in the circular buffer
//     tickBuffer[bufferIndex] = tickCount;
//     bufferIndex = (bufferIndex + 1) % bufferSize; // Move to the next index (circularly)

//     // Reset tick count for the next interval
//     tickCount = 0;
    
//     // Print the moving average RPM
//     Serial.print("Moving Average RPM: ");
//     Serial.println(calculateMovingAverageRPM());
//   }

//   // Check if UART data is available to read
//   if (mySerial.available()) {
//     // Read data and display it
//     String message = mySerial.readStringUntil('\n');
//     Serial.println("Received: " + message);
//   }
// }

// // Interrupt Service Routine (ISR) for counting ticks
// void countTick() {
//   tickCount++;
// }

// // Function to calculate moving average RPM
// float calculateMovingAverageRPM() {
//   unsigned int totalTicks = 0;
//   unsigned int samples = 0;

//   // Sum up ticks from the buffer (handling case where time < 1 second)
//   for (int i = 0; i < bufferSize; i++) {
//     totalTicks += tickBuffer[i];
//     if (tickBuffer[i] > 0) samples++;  // Count valid samples
//   }

//   // If we have samples, calculate the RPM
//   if (samples > 0) {
//     float ticksPerSecond = (totalTicks / (float)samples) * (1000.0 / sampleInterval);
//     return (ticksPerSecond / teethPerRevolution) * 60.0;
//   } else {
//     return 0.0; // Return 0 if no valid samples
//   }
// }


