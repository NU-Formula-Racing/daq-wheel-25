#include "wheel_temp.h"

// constructor
WheelTemp::WheelTemp(HardwareSerial& port) : serial(port) {}

// Returns true only when a new checksum-verified frame was accepted
bool WheelTemp::parseSerial() {
    bool newFrame = false;
    while (serial.available()) {
        uint8_t b = serial.read();
        switch (state) {
            // search for "N F R 2 5"
            case State::SEEK_PREAMBLE:
                if (b == PREAMBLE[preIndex]) {
                    frame[preIndex++] = b;
                    if (preIndex == PRE_LEN)  // found “NFR25”
                    {
                        state = State::COLLECT_FRAME;
                        byteIndex = PRE_LEN;
                    }
                } else {
                    preIndex = (b == PREAMBLE[0]) ? 1 : 0;
                    frame[0] = (preIndex == 1) ? b : 0;
                }
                break;

            // parse rest of framee
            case State::COLLECT_FRAME:
                frame[byteIndex++] = b;
                if (byteIndex == FRAME_LEN) {
                    Serial.println("Collected frame!");
                    Packet *packet = reinterpret_cast<Packet *>(&frame);
                    uint16_t calc = Packet::calculateChecksum(packet->temp);  // calculate checksum

                    // reset state for next search
                    state = State::SEEK_PREAMBLE;
                    preIndex = 0;
                    byteIndex = 0;

                    if (packet->checksum == calc) {
                        for (int i = 0; i < NUM_TEMPS; i++) {
                            lastPacket.temp[i] = packet->temp[i];
                        }
                    } else {
                        Serial.println("Checksum failed!");
                    }
                }
                break;
        }
    }
    return newFrame;
}

// just gets the temps
std::array<float, 8> WheelTemp::getTemps() {
    parseSerial();       // process bytes and put in latest payload
    return lastPacket.temp;
}