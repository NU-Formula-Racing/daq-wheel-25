#include <Arduino.h>
#include <array>
const int potPin = 36;
const float pot_const = 3.3/4095;
const float slope = 1380;
const int LUT_SIZE = 53;

// Example: Mapping voltage readings (in mV) to temperature (in Celsius)
const float to_bit[LUT_SIZE] = {
  0, 0, 0, 0, 30, 98, 216, 302, 352, 419, 488, 590, 654, 748, 834, 893, 970, 1053, 1152, 
  1232, 1292, 1388, 1472, 1589, 1644, 1718, 1802, 1888, 1956, 2050, 2126, 2218, 2277, 
  2374, 2453, 2521, 2615, 2725, 2795, 2872, 2950, 3030, 3133, 3238, 3337, 3485, 3600, 
  3723, 3843, 4000, 4095, 4095, 4095
};

float to_mm_lut[4096];

void setup() {
    Serial.begin(9600);
    pinMode(potPin, INPUT);
    
    int current_bit = 0; // to_mm_lut index pointer
    int last_mm_bit_val = 0; // to_bit index pointer
    while (current_bit != 4095) {
        // if (current < 10) {
        //     std::cout << std::endl;
        // }
        if (to_bit[last_mm_bit_val] == 4095) {
            // if at tail end dead zone
            to_mm_lut[current_bit] = 4095;
            current_bit += 1;
        } else if (to_bit[last_mm_bit_val + 1] == current_bit) {
            // if bit value is direct hit
            last_mm_bit_val += 1;
            to_mm_lut[current_bit] = to_bit[last_mm_bit_val];
        } else {
            // if interpolation required, bit value isn't direct hit
            float interp_num = (current_bit - to_bit[last_mm_bit_val]);
            float interp_denom = to_bit[last_mm_bit_val + 1] - to_bit[last_mm_bit_val];
            to_mm_lut[current_bit] = interp_num/interp_denom + last_mm_bit_val;
            current_bit += 1;
        }
    }
  
    Serial.println("Lookup table generated.");
}

void loop() {
  // Read the analog value from the potentiometer (0 to 1023)
  int potValue = analogRead(potPin);
  // Print value conversion to mm
  Serial.println(to_bit[potValue]);
  delay(100);
}

