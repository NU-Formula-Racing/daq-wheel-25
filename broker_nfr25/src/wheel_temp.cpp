#include "wheel_temp.h"

// constructor
WheelTemp::WheelTemp(HardwareSerial& port) : serial(port) { }

//Returns true only when a new checksum-verified frame was accepted
bool WheelTemp::parseSerial()
{
    bool newFrame = false;
    while (serial.available())
    {
        uint8_t b = serial.read();
        switch (state)
        {
            // search for "N F R 2 5" 
            case State::SEEK_PREAMBLE:
                if (b == PREAMBLE_STR[preIndex])
                {
                    frame[preIndex++] = b;
                    if (preIndex == PRE_LEN)          // found “NFR25”
                    {
                        state     = State::COLLECT_FRAME;
                        byteIndex = PRE_LEN;
                    }
                }
                else
                {
                    preIndex = (b == PREAMBLE_STR[0]) ? 1 : 0;
                    frame[0] = (preIndex == 1) ? b : 0;
                }
                break;

            // parse rest of framee
            case State::COLLECT_FRAME:
                frame[byteIndex++] = b;
                if (byteIndex == FRAME_LEN)
                {
                    uint16_t rx   = frame[FRAME_LEN-2] | (frame[FRAME_LEN-1] << 8); //recieved checksum
                    uint16_t calc = calculate_checksum(frame[FRAME_LEN-2]); //calculate checksum 

                    // reset state for next search
                    state = State::SEEK_PREAMBLE;
                    preIndex = 0;
                    byteIndex = 0;

                    if (rx == calc)
                    {
                        memcpy(&lastPacket, frame, FRAME_LEN);

                        // just put the payload only lol 
                        memcpy(lastPayload.data(),
                               frame + PRE_LEN,               // skip preamble
                               lastPayload.size());
                        newFrame = true;
                    }
                }
                break;
        }
    }
    return newFrame;
}

 // just gets the temps 
std::array<float,8> WheelTemp::getTemps()
{
    parseSerial();          // process bytes and put in latest payload
    return lastPayload;     // always return latest verified payload
}