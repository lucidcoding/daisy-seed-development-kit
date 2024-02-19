#include "PotentiometerToDisplayValueScaler.h"
#include <math.h>
#include <stdexcept>

namespace developmentKit::library::uiFramework::shared::scaling
{
    void PotentiometerToDisplayValueScaler::Init(int16_t prmMin, int16_t prmMax)
    {
        min = prmMin;
        max = prmMax;
    }

    int16_t PotentiometerToDisplayValueScaler::Scale(float in)
    {
        float floatValue = (in * (max + 1 - min));
        return (int16_t)floatValue + min;
    }

    float PotentiometerToDisplayValueScaler::GetDivision()
    {
        return 1.0f / (float)(max - min + 1);
    }
}