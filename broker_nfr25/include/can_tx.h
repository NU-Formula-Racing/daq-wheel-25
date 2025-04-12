#ifndef __CAN_TX_H__
#define __CAN_TX_H__

#include <CAN.h>

class CANTX {
   public:
    // put function declarations here:
    ESPCAN can{10U, GPIO_NUM_16, GPIO_NUM_4};
    VirtualTimerGroup timerGroup;

    MakeSignedCANSignal(float, 0, 16, 1.0, 0.0) wheelSpeedSignal;
    MakeSignedCANSignal(float, 16, 16, 1.0, 0.0) displacementSignal;
    MakeSignedCANSignal(float, 32, 16, 1.0, 0.0) loadSignal;

    CANTXMessage<3> flWheel{driveBus, 0x249, false, 6, 100, wheelSpeedSignal, displacementSignal, loadSignal};
    // CANTXMessage<3> frWheel{driveBus, 0x24A, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    // CANTXMessage<3> blWheel{driveBus, 0x24B, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    // CANTXMessage<3> brWheel{driveBus, 0x24C, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};

    void initialize() {
        driveBus.Initialize(ICAN::BaudRate::kBaud500K);
        timerGroup.AddTimer(100, [this]() { flWheel.EncodeAndSend(); });
    }

    void tick() {
        driveBus.Tick();
        timerGroup.Tick(millis());
    }
};

#endif  // __CAN_TX_H__