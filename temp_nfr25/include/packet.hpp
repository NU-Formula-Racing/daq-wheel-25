// packet.hpp

#include <stddef.h>
#include <stdint.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

constexpr std::array<uint8_t, 5> PREAMBLE = {'N', 'F', 'R', '2', '5'};
constexpr std::size_t NUM_TEMPS = 8;

struct Packet {
   public:
    uint8_t preamble[sizeof(PREAMBLE)];  // 5 bytes
    std::array<float, NUM_TEMPS> temp;
    std::uint16_t checksum;  // 2 bytes

    static Packet makePacket(const std::array<float, NUM_TEMPS> &temps) {
        Packet p;
        memcpy(p.preamble, PREAMBLE.data(), PREAMBLE.size());
        for (std::size_t i = 0; i < NUM_TEMPS; ++i)
            p.temp[i] = temps[i];
        p.checksum = Packet::calculateChecksum(temps);

        return p;
    }

    static std::uint16_t calculateChecksum(const std::array<float, NUM_TEMPS> &temp_readings) {
        uint16_t checksum = 0;

        for (size_t i = 0; i < temp_readings.size(); ++i) {
            checksum ^= static_cast<int16_t>(temp_readings[i]);  // XOR each temperature reading
        }

        return checksum;
    }
};
