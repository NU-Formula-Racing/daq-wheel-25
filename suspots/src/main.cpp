#include <Arduino.h>
#include <array>
const int potPin = 36;
const float pot_const = 3.3/4095;
const float slope = 1380;

// Example: Mapping voltage readings (in mV) to temperature (in Celsius)
// const std::array<std::pair<int, float>, 5> temperature_lookup_table = {
//     {{100, 20.0},  // 100mV corresponds to 20.0 degrees Celsius
//      {200, 25.0},
//      {300, 30.0},
//      {400, 35.0},
//      {500, 40.0}}
// };
void setup() {
    Serial.begin(9600);
    pinMode(potPin, INPUT);
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
}

