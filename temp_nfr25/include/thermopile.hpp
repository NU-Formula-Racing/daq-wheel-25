#ifndef __THERMOPILE_H__
#define __THERMOPILE_H__

#include <Arduino.h>

#include <define.hpp>
#include <lut.hpp>
#include <memory>

struct ThermopileConfig {
   public:
    float thermistorBeta;

    // see hardware -- voltage divider settings
    float thermistorVin;
    float thermistorR2;
    float thermistorRoomTempResistance;

    // see hardware -- options from instrumentation amp
    float thermopileSignalOffset;
    float thermopileSignalGain;

    // something that we find and calibrate
    float emissivity;
    float thermopileFactor;

    // see ZTB
    std::shared_ptr<NumericLUT> thermopileLut;
};

class Thermopile {
   public:
    Thermopile() : _thermopileSignal(HWPin::NONE), _thermistorSignal(HWPin::NONE), _config() {}
    Thermopile(HWPin thermopileSignal, HWPin thermistorSignal, ThermopileConfig config) : _thermopileSignal(thermopileSignal), _thermistorSignal(thermistorSignal), _config(config) {}

    void initalize() {
        pinMode(_thermopileSignal, INPUT);
        pinMode(_thermistorSignal, INPUT);
    }

    float getAmbientTemperature() {
        // Read divider output
        const float vOut = readVoltage(_thermistorSignal);

        // Protect against division-by-zero or a short to GND
        if (vOut <= 0.0f) {
            return NAN;
        }

        // Convert VOUT → thermistor resistance
        // R_therm = (R2 * Vin / Vout) − R2
        const float rTherm = (_config.thermistorR2 * _config.thermistorVin / vOut) - _config.thermistorR2;

        // Guard against open-circuit or overflow
        if (rTherm <= 0.0f) {
            return NAN;
        }

        // Beta equation (Kelvin)
        constexpr float T0_K = 298.15f;  // 25 °C in kelvin
        const float lnRatio = logf(rTherm / _config.thermistorRoomTempResistance);

        const float invT = (1.0f / T0_K) + (lnRatio / _config.thermistorBeta);
        const float tempK = 1.0f / invT;

        // Convert to Celsius and return
        return tempK - 273.15f;
    }

    float getObjectTemperature() {
        // raw thermopile amplifier output → true sensor voltage Vtp
        const float voltageThermopile = getThermopileVoltage();

        // bail if something went horribly wrong
        if (!isfinite(voltageThermopile)) {
            return NAN;
        }

        // measure ambient temperature (°C) via the thermistor
        const float ambientTemperature = getAmbientTemperature();
        if (!isfinite(ambientTemperature)) {
            return NAN;
        }

        // find the voltage we should compensate with
        // what is the voltage that can be attributed to the ambient temperature?
        const float voltageAmbient = _config.thermopileLut->getInverseValue(ambientTemperature);

        // compensate with that voltage
        const float voltageCompensated = voltageThermopile + voltageAmbient;

        // 5) invert the same LUT to go from fObject → To (°C)
        const float tempObject = _config.thermopileLut->getValue(voltageCompensated);

        return tempObject;
    }

    float getThermopileVoltage() {
                // raw thermopile amplifier output → true sensor voltage Vtp
                const float frontendOutput = readVoltage(_thermopileSignal);
                const float voltageThermopile = (frontendOutput - _config.thermopileSignalOffset) / _config.thermopileSignalGain;
                return voltageThermopile;
    }

    Thermopile &operator=(const Thermopile &other) = default;

   private:
    float readVoltage(HWPin pin) {
        double reading = analogRead(pin);  // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
        if (reading < 1 || reading > 4095) return 0;
        return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
    }

    HWPin _thermopileSignal;
    HWPin _thermistorSignal;
    ThermopileConfig _config;
};

#endif  // __THERMOPILE_H__