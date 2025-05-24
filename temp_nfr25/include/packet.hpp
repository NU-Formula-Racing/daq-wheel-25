#include <memory>
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

const std::array<uint8_t, 5> PREAMBLE = { 'N', 'F', 'R', '2', '5' };
const std::size_t NUM_TEMPS = 8;

struct Packet {
    uint8_t preamble[sizeof(PREAMBLE)]; // 5 bytes
    float temp[NUM_TEMPS]; //8 * 4 bytes
    uint16_t checksum; // 2 bytes
}

uint16_t calculate_checksum(std::vector<float> temp_readings) {
    uint16_t checksum = 0;

    for (size_t i = 0; i < temp_readings.size(); ++i) {
        checksum ^= static_cast<int16_t>(temp_readings[i]);  // XOR each temperature reading
    }

    return checksum;
}

Packet make_packet(const std::array<float, NUM_TEMPS>& temps)
{
    Packet p;
    memcpy(p.preamble, PREAMBLE.data(), PREAMBLE.size());
    for (std::size_t i = 0; i < NUM_TEMPS; ++i)
        p.temps[i] = temps[i];
    p.checksum = calculate_checksum(p.temps);

    return p;
}