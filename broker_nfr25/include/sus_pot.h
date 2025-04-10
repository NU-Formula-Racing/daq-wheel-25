#ifndef __SUS_POT_H__
#define __SUS_POT_H__

#include "define.h"
#include "lut.h"

class SusPot {
   public:
    SusPot(HWPin analogPin, float r2, NumericLUT voltageToDisplacement);

    void initialize();

    float getDisplacement();

   private:
    HWPin _analogPin;
    float _r2;
    NumericLUT _voltageToDisplacementLut;
};

#endif  // __SUS_POT_H__