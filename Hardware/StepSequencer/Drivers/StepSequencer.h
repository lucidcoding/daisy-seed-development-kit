#pragma once
#ifndef STEP_SEQUENCER_H
#define STEP_SEQUENCER_H
#define STEP_SEQUENCER_PROCESS_INTERVAL_US 500

#define STEP_SEQUENCER_MODE_STOP 0
#define STEP_SEQUENCER_MODE_PLAY 1
#define STEP_SEQUENCER_MODE_STEP_REC 2

#include "stdint.h"
#include "Step.h"

namespace developmentKit::stepSequencer
{
    class StepSequencer
    {
    public:
        void Init();
        void SetKeys(uint8_t keys);
        void Process(uint32_t currentProcessTimeUs);
        uint64_t GetLedStates();

    private:
        uint8_t stepCount;
        uint8_t currentStep;
        Step steps[16];
        uint8_t lastKeyPress;
        uint16_t tick;
        uint16_t stepInterval;
        void UpdateLedsForCurrentStep();
        void StartStepRecording();
        uint8_t mode;
        const uint8_t noteToLedLookup[13] = {9, 0, 10, 1, 11, 12, 2, 13, 3, 14, 4, 15, 16};
        bool leds[32];
        uint8_t getNoteFromKeyPressed(uint8_t keyPressed);
    };
}

#endif