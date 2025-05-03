#ifndef __CAN_TX_H__
#define __CAN_TX_H__

#include <CAN.h>
#include <virtualTimer.h>

#include "define.h"

enum BrokerPosition {
    BP_FL,
    BP_FR,
    BP_BL,
    BP_BR
};

class CANTX {
   public:
    // put function declarations here:
    ESPCAN driveBus{10U, GPIO_NUM_16, GPIO_NUM_4};
    VirtualTimerGroup timerGroup;
    BrokerPosition position;

    MakeSignedCANSignal(float, 0, 16, 1.0, 0.0) wheelSpeedSignal;
    MakeSignedCANSignal(float, 16, 16, 1.0, 0.0) displacementSignal;
    MakeSignedCANSignal(float, 32, 16, 1.0, 0.0) loadSignal;

    CANTXMessage<3> flWheel{driveBus, 0x249, false, 6, 100, wheelSpeedSignal, displacementSignal, loadSignal};
    CANTXMessage<3> frWheel{driveBus, 0x24A, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    CANTXMessage<3> blWheel{driveBus, 0x24B, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    CANTXMessage<3> brWheel{driveBus, 0x24C, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};

    void initialize() {
        driveBus.Initialize(ICAN::BaudRate::kBaud500K);
        timerGroup.AddTimer(100, [this]() {
            Serial.println("Sending message!");
            // figure out what wheel we are
            switch (this->position) {
                case BrokerPosition::BP_FL:
                    flWheel.EncodeAndSend();
                    break;
                case BrokerPosition::BP_FR:
                    frWheel.EncodeAndSend();
                    break;
                case BrokerPosition::BP_BL:
                    blWheel.EncodeAndSend();
                    break;
                case BrokerPosition::BP_BR:
                    brWheel.EncodeAndSend();
                    break;
            }
        });

        Serial.println("Reading Settings...");
        pinMode(HWPin::FB_SETTING, INPUT);
        pinMode(HWPin::LR_SETTING, INPUT);
        pinMode(HWPin::FB_INDICATOR, OUTPUT);
        pinMode(HWPin::LR_INDICATOR, OUTPUT);

        // figure out the broker positions
        bool front = digitalRead(HWPin::FB_SETTING) == HIGH;
        bool left = digitalRead(HWPin::LR_SETTING) == HIGH;

        if (front && left)
            position = BrokerPosition::BP_FL;
        else if (!front && left)
            position = BrokerPosition::BP_BL;
        else if (front && !left)
            position = BrokerPosition::BP_FR;
        else if (!front && !left)
            position = BrokerPosition::BP_BR;
        else
            position = BrokerPosition::BP_FL;

        // write the indciators
        digitalWrite(HWPin::FB_INDICATOR, front ? HIGH : LOW);
        digitalWrite(HWPin::LR_INDICATOR, left ? HIGH : LOW);

        Serial.print("Wheel Configuration: ");
        Serial.println(front ? (left ? "Front-Left" : "Front-Right") : (left ? "Back-Left" : "Back-Right"));
    }

    void tick() {
        driveBus.Tick();
        timerGroup.Tick(millis());
    }
};

#endif  // __CAN_TX_H__