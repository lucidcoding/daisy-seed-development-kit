#pragma once
#ifndef STEP_SEQUENCER_BRAIN_H
#define STEP_SEQUENCER_BRAIN_H
#define STEP_SEQUENCER_PROCESS_INTERVAL_US 250
#define STEP_SEQUENCER_NUMBER_OF_LEDS 23
#define STEP_SEQUENCER_NUMBER_OF_NOTE_KEYS 13
#define STEP_SEQUENCER_NO_KEY_PRESS 255
#define STEP_SEQUENCER_NOT_NOTE_KEY 255

#define STEP_SEQUENCER_MODE_STOP 0
#define STEP_SEQUENCER_MODE_PLAY 1
#define STEP_SEQUENCER_MODE_STEP_REC 2

#define STEP_SEQUENCER_KEYS_FUNC 5
#define STEP_SEQUENCER_KEYS_PLAY 6
#define STEP_SEQUENCER_KEYS_REC 7
#define STEP_SEQUENCER_KEYS_MEMORY 8
#define STEP_SEQUENCER_KEYS_C 9
#define STEP_SEQUENCER_KEYS_C_SHARP 0
#define STEP_SEQUENCER_KEYS_D 10
#define STEP_SEQUENCER_KEYS_D_SHARP 1
#define STEP_SEQUENCER_KEYS_E 11
#define STEP_SEQUENCER_KEYS_F 12
#define STEP_SEQUENCER_KEYS_F_SHARP 2
#define STEP_SEQUENCER_KEYS_G 13
#define STEP_SEQUENCER_KEYS_G_SHARP 3
#define STEP_SEQUENCER_KEYS_A 14
#define STEP_SEQUENCER_KEYS_A_SHARP 4
#define STEP_SEQUENCER_KEYS_B 15
#define STEP_SEQUENCER_KEYS_C2 16
#define STEP_SEQUENCER_KEYS_OCTAVE_DOWN 17
#define STEP_SEQUENCER_KEYS_OCTAVE_UP 18
#define STEP_SEQUENCER_KEYS_ACCENT 19
#define STEP_SEQUENCER_KEYS_SLIDE 20
#define STEP_SEQUENCER_KEYS_BACK 21
#define STEP_SEQUENCER_KEYS_NEXT 22

#define STEP_SEQUENCER_LEDS_FUNC 5
#define STEP_SEQUENCER_LEDS_PLAY 6
#define STEP_SEQUENCER_LEDS_REC 7
#define STEP_SEQUENCER_LEDS_MEMORY 8
#define STEP_SEQUENCER_LEDS_C 9
#define STEP_SEQUENCER_LEDS_C_SHARP 0
#define STEP_SEQUENCER_LEDS_D 10
#define STEP_SEQUENCER_LEDS_D_SHARP 1
#define STEP_SEQUENCER_LEDS_E 11
#define STEP_SEQUENCER_LEDS_F 12
#define STEP_SEQUENCER_LEDS__SHARP 2
#define STEP_SEQUENCER_LEDS_G 13
#define STEP_SEQUENCER_LEDS_G_SHARP 3
#define STEP_SEQUENCER_LEDS_A 14
#define STEP_SEQUENCER_LEDS_A_SHARP 4
#define STEP_SEQUENCER_LEDS_B 15
#define STEP_SEQUENCER_LEDS_C2 16
#define STEP_SEQUENCER_LEDS_OCTAVE_DOWN 17
#define STEP_SEQUENCER_LEDS_OCTAVE_UP 18
#define STEP_SEQUENCER_LEDS_ACCENT 19
#define STEP_SEQUENCER_LEDS_SLIDE 20
#define STEP_SEQUENCER_LEDS_BACK 21
#define STEP_SEQUENCER_LEDS_NEXT 22

#include "stdint.h"
#include "Step.h"
#include "NoteEvent.h"

namespace developmentKit::stepSequencer
{
    class SequencerBrain
    {
    public:
        void Init();
        void SetKeys(uint8_t keys);
        void Process(uint32_t currentProcessTimeUs);

        // For testing only
        uint8_t GetCurrentStepIndex();
        Step GetCurrentStep();
        uint8_t GetMode();

        uint64_t GetLedStates();
        bool GetGate();
        void SetStepInterval(uint8_t newStepInterval);
        void SetSteps(Step newSteps[16]);

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
        const uint8_t noteToLedLookup[STEP_SEQUENCER_NUMBER_OF_NOTE_KEYS] = {9, 0, 10, 1, 11, 12, 2, 13, 3, 14, 4, 15, 16};
        bool leds[STEP_SEQUENCER_NUMBER_OF_LEDS];
        uint8_t GetNoteFromKeyPressed(uint8_t keyPressed);
        uint8_t gateCount;
        bool gateOn;
        uint8_t gateLength;
        void ActivateCurrentStep();
    };
}

#endif