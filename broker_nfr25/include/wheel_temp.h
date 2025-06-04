#pragma once
#include "define.h"
#include <HardwareSerial.h>
#include <stdint.h>
#include <string.h>
#include "packet.hpp"

class WheelTemp
{
public:
    explicit WheelTemp(HardwareSerial &port);
    std::array<float, 8> getTemps(); // returns temp data into array of 8 floats

private:
    bool parseSerial(); // for getting and storing temp data

    enum class State : uint8_t
    {
        SEEK_PREAMBLE,
        COLLECT_FRAME
    }; // storing state of parsing

    static constexpr char PREAMBLE_STR[6] = "NFR25";
    static constexpr size_t PRE_LEN = sizeof(PREAMBLE_STR) - 1; // 5
    static constexpr size_t CRC_LEN = 2;                        // checksum length double check w kris
    static constexpr size_t FRAME_LEN = sizeof(Packet);     // change to whatever kris did
    static_assert(FRAME_LEN >= PRE_LEN + CRC_LEN, "TempPacket too small!");

    HardwareSerial &serial;

    // parser bookkeeping
    State state = State::SEEK_PREAMBLE;
    size_t preIndex = 0;
    size_t byteIndex = 0;
    uint8_t frame[FRAME_LEN];

    // verified data
    Packet lastPacket{};
    std::array<float, 8> lastPayload{{}};
};