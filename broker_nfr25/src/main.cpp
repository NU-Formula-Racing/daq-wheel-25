#include <Arduino.h>
//For first drive:
  //IO 27 for sus pots (jumpered to ADC pin)
  //IO22 for wheel speed (digital signal, no ADC needed)

// Sus-pot Constants
const int potPin = 27;
const float pot_const = 3.3/4095;
const float slope = 1380;

// Wheel Speed Setup
  // Constants
const int wheel_speed_pin = 22;               // Pin connected to the sensor output
const int teethPerRevolution = 70;      // Number of teeth per revolution
const int sampleInterval = 100;         // Sampling interval in milliseconds
const int bufferSize = 10;              // Number of intervals for moving average
  // Variables
volatile unsigned int tickCount = 0;         // Counts pulses from the sensor in the current interval
unsigned int tickBuffer[bufferSize] = {0};     // Circular buffer for storing tick counts
unsigned int bufferIndex = 0;                // Current index in the buffer
unsigned long lastSampleTime = 0;            // Time of last sample
float movingAverageRPM = 0;                  // Calculated moving average RPM


// Interrupt Service Routine for counting ticks (each falling edge corresponds to one pulse)
void countTick() {
  tickCount++;
}

// Function to calculate moving average RPM from the tick buffer
float calculateMovingAverageRPM() {
  unsigned long totalTicks = 0;
  // Sum up tick counts from every sample interval in the buffer.
  for (int i = 0; i < bufferSize; i++) {
    totalTicks += tickBuffer[i];
  }
  // Calculate average ticks per interval (even if some intervals recorded zero ticks)
  float avgTicksPerSample = totalTicks / (float)bufferSize;
  
  // Convert to ticks per second
  float ticksPerSecond = avgTicksPerSample * (1000.0 / sampleInterval);
  
  // Calculate RPM (ticks per second divided by teeth per revolution gives RPS; multiplied by 60 for RPM)
  return (ticksPerSecond / teethPerRevolution) * 60.0;
}

void setup() {
    Serial.begin(115200);
    pinMode(potPin, INPUT);
    pinMode(wheel_speed_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(wheel_speed_pin), countTick, FALLING);
    lastSampleTime = millis();
}

void loop() {
  // Read the analog value from the potentiometer (0 to 1023)
  float potValue = analogRead(potPin);
  float voltage = pot_const * potValue;
  float distance = potValue/slope;
  // Print the potentiometer value to the Serial Monitor
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);
  Serial.print("Voltage: ");
  Serial.println(voltage);
  Serial.print("Distance:");
  Serial.println(distance);
  delay(100);

  unsigned long currentTime = millis();
  
    // Check if it's time to sample the current tick count
    if (currentTime - lastSampleTime >= sampleInterval) {
      // Safely copy and reset the tick counter
      noInterrupts();
      tickBuffer[bufferIndex] = tickCount;
      tickCount = 0;
      interrupts();
      
      // Move to the next buffer index, wrapping around if necessary
      bufferIndex = (bufferIndex + 1) % bufferSize;
      lastSampleTime = currentTime;
      
      // Compute the moving average RPM and print it over Serial
      movingAverageRPM = calculateMovingAverageRPM();
      Serial.print("Moving Average RPM: ");
      Serial.println(movingAverageRPM);
    }
    // // Read the sensor value.
    // // LOW indicates that metal is detected.
    // bool metalDetected = (digitalRead(sensorPin) == LOW);
    
    // if (metalDetected) {
    //   Serial.println("Metal detected");
    // } else {
    //   Serial.println("No metal detected");
    // }
    
    // // Delay to reduce serial output rate.
    // delay(100);
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


