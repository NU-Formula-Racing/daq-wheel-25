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

    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_back_left_inner_temps_bli_temperature_4{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_back_left_inner_temps_bli_temperature_5{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_back_left_inner_temps_bli_temperature_6{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_back_left_inner_temps_bli_temperature_7{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_back_left_outer_temps_blo_temperature_0{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_back_left_outer_temps_blo_temperature_1{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_back_left_outer_temps_blo_temperature_2{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_back_left_outer_temps_blo_temperature_3{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_back_right_inner_temps_bri_temperature_4{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_back_right_inner_temps_bri_temperature_5{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_back_right_inner_temps_bri_temperature_6{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_back_right_inner_temps_bri_temperature_7{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_back_right_outer_temps_bro_temperature_0{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_back_right_outer_temps_bro_temperature_1{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_back_right_outer_temps_bro_temperature_2{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_back_right_outer_temps_bro_temperature_3{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_front_left_inner_temps_fli_temperature_4{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_front_left_inner_temps_fli_temperature_5{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_front_left_inner_temps_fli_temperature_6{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_front_left_inner_temps_fli_temperature_7{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_front_left_outer_temps_flo_temperature_0{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_front_left_outer_temps_flo_temperature_1{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_front_left_outer_temps_flo_temperature_2{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_front_left_outer_temps_flo_temperature_3{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_front_right_inner_temps_fri_temperature_4{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_front_right_inner_temps_fri_temperature_5{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_front_right_inner_temps_fri_temperature_6{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_front_right_inner_temps_fri_temperature_7{};
    MakeSignedCANSignal(float, 0, 16, 0.01, 0.0) daq_wheel_front_right_outer_temps_fro_temperature_0{};
    MakeSignedCANSignal(float, 16, 16, 0.01, 0.0) daq_wheel_front_right_outer_temps_fro_temperature_1{};
    MakeSignedCANSignal(float, 32, 16, 0.01, 0.0) daq_wheel_front_right_outer_temps_fro_temperature_2{};
    MakeSignedCANSignal(float, 48, 16, 0.01, 0.0) daq_wheel_front_right_outer_temps_fro_temperature_3{};

    CANTXMessage<3> flWheel{driveBus, 0x249, false, 6, 100, wheelSpeedSignal, displacementSignal, loadSignal};
    CANTXMessage<3> frWheel{driveBus, 0x24A, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    CANTXMessage<3> blWheel{driveBus, 0x24B, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    CANTXMessage<3> brWheel{driveBus, 0x24C, 6, 0, wheelSpeedSignal, displacementSignal, loadSignal};
    
    CANTXMessage<4> daq_wheel_back_left_inner_temps{driveBus, 0x246, 8, 0, daq_wheel_back_left_inner_temps_bli_temperature_4, daq_wheel_back_left_inner_temps_bli_temperature_5, daq_wheel_back_left_inner_temps_bli_temperature_6, daq_wheel_back_left_inner_temps_bli_temperature_7};
    CANTXMessage<4> daq_wheel_back_left_outer_temps{driveBus, 0x245, 8, 0, daq_wheel_back_left_outer_temps_blo_temperature_0, daq_wheel_back_left_outer_temps_blo_temperature_1, daq_wheel_back_left_outer_temps_blo_temperature_2, daq_wheel_back_left_outer_temps_blo_temperature_3};
    CANTXMessage<4> daq_wheel_back_right_inner_temps{driveBus, 0x248, 8, 0, daq_wheel_back_right_inner_temps_bri_temperature_4, daq_wheel_back_right_inner_temps_bri_temperature_5, daq_wheel_back_right_inner_temps_bri_temperature_6, daq_wheel_back_right_inner_temps_bri_temperature_7};
    CANTXMessage<4> daq_wheel_back_right_outer_temps{driveBus, 0x247, 8, 0, daq_wheel_back_right_outer_temps_bro_temperature_0, daq_wheel_back_right_outer_temps_bro_temperature_1, daq_wheel_back_right_outer_temps_bro_temperature_2, daq_wheel_back_right_outer_temps_bro_temperature_3};
    CANTXMessage<4> daq_wheel_front_left_inner_temps{driveBus, 0x242, 8, 0, daq_wheel_front_left_inner_temps_fli_temperature_4, daq_wheel_front_left_inner_temps_fli_temperature_5, daq_wheel_front_left_inner_temps_fli_temperature_6, daq_wheel_front_left_inner_temps_fli_temperature_7};
    CANTXMessage<4> daq_wheel_front_left_outer_temps{driveBus, 0x241, 8, 0, daq_wheel_front_left_outer_temps_flo_temperature_0, daq_wheel_front_left_outer_temps_flo_temperature_1, daq_wheel_front_left_outer_temps_flo_temperature_2, daq_wheel_front_left_outer_temps_flo_temperature_3};
    CANTXMessage<4> daq_wheel_front_right_inner_temps{driveBus, 0x244, 8, 0, daq_wheel_front_right_inner_temps_fri_temperature_4, daq_wheel_front_right_inner_temps_fri_temperature_5, daq_wheel_front_right_inner_temps_fri_temperature_6, daq_wheel_front_right_inner_temps_fri_temperature_7};
    CANTXMessage<4> daq_wheel_front_right_outer_temps{driveBus, 0x243, 8, 0, daq_wheel_front_right_outer_temps_fro_temperature_0, daq_wheel_front_right_outer_temps_fro_temperature_1, daq_wheel_front_right_outer_temps_fro_temperature_2, daq_wheel_front_right_outer_temps_fro_temperature_3};

    void initialize() {
        driveBus.Initialize(ICAN::BaudRate::kBaud500K);
        timerGroup.AddTimer(100, [this]() {
            // Serial.println("Sending message!");
            // figure out what wheel we are
            switch (this->position) {
                case BrokerPosition::BP_FL:
                    flWheel.EncodeAndSend();
                    daq_wheel_front_left_inner_temps.EncodeAndSend();
                    daq_wheel_front_left_outer_temps.EncodeAndSend();
                    break;
                case BrokerPosition::BP_FR:
                    frWheel.EncodeAndSend();
                    daq_wheel_front_right_inner_temps.EncodeAndSend();
                    daq_wheel_front_right_outer_temps.EncodeAndSend();
                    break;
                case BrokerPosition::BP_BL:
                    blWheel.EncodeAndSend();
                    daq_wheel_back_left_inner_temps.EncodeAndSend();
                    daq_wheel_back_left_outer_temps.EncodeAndSend();
                    break;
                case BrokerPosition::BP_BR:
                    brWheel.EncodeAndSend();
                    daq_wheel_back_right_inner_temps.EncodeAndSend();
                    daq_wheel_back_right_outer_temps.EncodeAndSend();
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