#include "PotentiometerArray.h"
#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::hardware::potentiometerArray::drivers
{
    using namespace daisysp;
    using namespace daisy;

    void PotentiometerArray::Init()
    {
        adcConfig[0].InitMux(seed->GetPin(MUX1_COM_PIN), 8, seed->GetPin(MUX_S0_PIN), seed->GetPin(MUX_S1_PIN), seed->GetPin(MUX_S2_PIN));
        adcConfig[1].InitMux(seed->GetPin(MUX2_COM_PIN), 8, seed->GetPin(MUX_S0_PIN), seed->GetPin(MUX_S1_PIN), seed->GetPin(MUX_S2_PIN));
        seed->adc.Init(adcConfig, 2);

        for (int i = 0; i < POTENTIOMETER_COUNT; i++)
        {
            analogControl[i].Init(&rawReading[i], this->seed->AudioCallbackRate());
        }
    }

    void PotentiometerArray::Process()
    {
        for (int muxIx = 0; muxIx < 2; muxIx++)
        {
            for (int muxPotIx = 0; muxPotIx < 8; muxPotIx++)
            {
                int globalPotIx = inputToIndexMapping[muxIx][muxPotIx];
                rawReading[globalPotIx] = seed->adc.GetMux(muxIx, muxPotIx);
                analogControl[globalPotIx].Process();
            }
        }
    }

    float *PotentiometerArray::GetValues()
    {
        float values[16];

        for (int i = 0; i < POTENTIOMETER_COUNT; i++)
        {
            values[i] = analogControl[i].GetRawFloat();
        }

        return values;
    }
}
