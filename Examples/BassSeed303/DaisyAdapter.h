#pragma once
#ifndef BASS_SEED_303_DAISY_ADAPTER_H
#define BASS_SEED_303_DAISY_ADAPTER_H

#include "../../Hardware/StepSequencer/Drivers/IHardware.h"
#include "../../Hardware/StepSequencer/Drivers/Step.h"

using namespace developmentKit::hardware::stepSequencer::drivers;

class DaisyAdapter : public IHardware
{
public:
    void SavePatterns(Step *patterns) { return; }

private:
};

#endif