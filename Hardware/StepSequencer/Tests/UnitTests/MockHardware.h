#pragma once
#ifndef STEP_SEQUENCER_MOCK_HARDWARE_H
#define STEP_SEQUENCER_MOCK_HARDWARE_H

#include "stdint.h"
#include "../../Drivers/Controller.h"
#include "../../Drivers/IHardware.h"
#include "../../Drivers/Step.h"

namespace developmentKit::hardware::stepSequencer::tests::unitTests
{
    using namespace developmentKit::hardware::stepSequencer::drivers;

    class MockHardware : public IHardware
    {
    public:
        void Clear()
        {
            savePatternsCount = 0;

            for (uint8_t savedStepIndex = 0; savedStepIndex < 128; savedStepIndex++)
            {
                savedPatterns[savedStepIndex].note = 0;
                savedPatterns[savedStepIndex].gate = true;
                savedPatterns[savedStepIndex].octaveDown = false;
                savedPatterns[savedStepIndex].octaveUp = false;
                savedPatterns[savedStepIndex].accent = false;
                savedPatterns[savedStepIndex].slide = false;
            }
        }
        void SavePatterns(Step *patterns)
        {
            savePatternsCount++;

            for (uint8_t savedStepIndex = 0; savedStepIndex < 128; savedStepIndex++)
            {
                savedPatterns[savedStepIndex].note = patterns[savedStepIndex].note;
                savedPatterns[savedStepIndex].gate = patterns[savedStepIndex].gate;
                savedPatterns[savedStepIndex].octaveDown = patterns[savedStepIndex].octaveDown;
                savedPatterns[savedStepIndex].octaveUp = patterns[savedStepIndex].octaveUp;
                savedPatterns[savedStepIndex].accent = patterns[savedStepIndex].accent;
                savedPatterns[savedStepIndex].slide = patterns[savedStepIndex].slide;
            }
        };
        uint8_t savePatternsCount = 0;
        Step savedPatterns[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT * 8];

    private:
    };
}

#endif