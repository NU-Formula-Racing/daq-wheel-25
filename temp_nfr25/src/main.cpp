#include <Arduino.h>
#include <memory>
#include <iostream>
#include <vector>

#include "thermopile.hpp"
#include "define.hpp"
#include "packet.hpp"
// #include "message.hpp"

#define ADC_CONST 3.3 / 4096;

std::array<Thermopile, NUM_SENSORS> g_sensors;

// struct Message {
//   std::vector<float> temps;
//   uint64_t checksum;

//   Message(const std::vector<float>& temp_vector, uint64_t checksum)
//       : type(type), temps(temp_vector), checksum(checksum) {}
// };

// uint16_t calculate_checksum(std::vector<float> temp_readings) {
//   uint16_t checksum = 0;

//   for (size_t i = 0; i < temp_readings.size(); ++i) {
//       checksum ^= static_cast<int16_t>(temp_readings[i]);  // XOR each temperature reading
//   }

//   return checksum;
// }

// void sendMessage(const Message& message) {
//   // Start by sending the message type
//   Serial.print(static_cast<uint8_t>(message.type));  // Convert enum to its underlying integer value
  
//   // Send temperatures (each float as a string)
//   for (size_t i = 0; i < message.temps.size(); ++i) {
//       Serial.print(message.temps[i], 2);  // Send temperature with 2 decimal places
//       if (i < message.temps.size() - 1) {
//           Serial.print(",");  // Separate values by commas
//       }
//   }

//   // Send the checksum
//   Serial.print(",");
//   Serial.println(message.checksum);
// }



void setup() {
    Serial.begin(115200);

    ThermopileConfig config = {
        .thermistorBeta = 3960,
        .thermistorVin = 3.3,
        .thermistorR2 = 10000,
        .thermistorRoomTempResistance = 100000,
        .thermopileSignalOffset = 0.55,
        .thermopileSignalGain = 246,
        .calibrationConstant = (1.6 * 1.6e-12),
        .thermopileLut = std::make_shared<NumericLUT>(__thermopileLUT)
    };

    // create the sensors
    for (int i = 0; i < NUM_SENSORS; i++) {
        new (&g_sensors[i]) Thermopile(
            getThermopilePin(i),
            getThermistorPin(i),
            config
        );
    }
}

void loop() {
    // float averageThemistorValue = 0;
    std::vector<float> current_temps;
    for (int i = 0; i < NUM_SENSORS; i++) {
        Thermopile thermopile = g_sensors[i];
        current_temps.push_back(thermopile.getObjectTemperature());
        //Serial.printf("%0.2fC (%0.4fmv) %0.2fC | ", thermopile.getObjectTemperature(), thermopile.getThermopileVoltage() * 1000, thermopile.getAmbientTemperature());
    }

    // averageThemistorValue /= NUM_SENSORS;
    // Serial.print(" | ");
    // Serial.print(averageThemistorValue);
    Serial.print("\n");
    
    Packet packet = make_packet(currentTemps);
    Serial.write(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));
    delay(100);
}