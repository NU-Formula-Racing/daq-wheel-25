#ifndef __STRAIN_GAUGE_H__
#define __STRAIN_GAUGE_H__

#include "define.h"
#include "lut.h"

class StrainGauge {
   public:
    StrainGauge(HWPin analogPin, float gain, float offset);

    void initialize();

    float getStrainSignal();

   private:
    HWPin _analogPin;
    float _offset;
    float _gain;
};

#endif  // __STRAIN_GAUGE_H__