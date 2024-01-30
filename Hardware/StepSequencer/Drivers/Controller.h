#pragma once
#ifndef STEP_SEQUENCER_CONTROLLER_H
#define STEP_SEQUENCER_CONTROLLER_H
#define STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT 16

#define STEP_SEQUENCER_CONTROLLER_MODE_STOP 0
#define STEP_SEQUENCER_CONTROLLER_MODE_PLAY 1
#define STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC 2

#define STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS 23
#define STEP_SEQUENCER_CONTROLLER_NUMBER_OF_NOTE_KEYS 13
#define STEP_SEQUENCER_CONTROLLER_NUMBER_OF_KEYS 23
#define STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS 255
#define STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY 255

#define STEP_SEQUENCER_CONTROLLER_KEYS_FUNC 5
#define STEP_SEQUENCER_CONTROLLER_KEYS_PLAY 6
#define STEP_SEQUENCER_CONTROLLER_KEYS_REC 7
#define STEP_SEQUENCER_CONTROLLER_KEYS_MEMORY 8
#define STEP_SEQUENCER_CONTROLLER_KEYS_C 9
#define STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP 0
#define STEP_SEQUENCER_CONTROLLER_KEYS_D 10
#define STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP 1
#define STEP_SEQUENCER_CONTROLLER_KEYS_E 11
#define STEP_SEQUENCER_CONTROLLER_KEYS_F 12
#define STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP 2
#define STEP_SEQUENCER_CONTROLLER_KEYS_G 13
#define STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP 3
#define STEP_SEQUENCER_CONTROLLER_KEYS_A 14
#define STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP 4
#define STEP_SEQUENCER_CONTROLLER_KEYS_B 15
#define STEP_SEQUENCER_CONTROLLER_KEYS_C2 16
#define STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN 17
#define STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP 18
#define STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT 19
#define STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE 20
#define STEP_SEQUENCER_CONTROLLER_KEYS_BACK 21
#define STEP_SEQUENCER_CONTROLLER_KEYS_NEXT 22

#define STEP_SEQUENCER_CONTROLLER_LEDS_FUNC 5
#define STEP_SEQUENCER_CONTROLLER_LEDS_PLAY 6
#define STEP_SEQUENCER_CONTROLLER_LEDS_REC 7
#define STEP_SEQUENCER_CONTROLLER_LEDS_MEMORY 8
#define STEP_SEQUENCER_CONTROLLER_LEDS_C 9
#define STEP_SEQUENCER_CONTROLLER_LEDS_C_SHARP 0
#define STEP_SEQUENCER_CONTROLLER_LEDS_D 10
#define STEP_SEQUENCER_CONTROLLER_LEDS_D_SHARP 1
#define STEP_SEQUENCER_CONTROLLER_LEDS_E 11
#define STEP_SEQUENCER_CONTROLLER_LEDS_F 12
#define STEP_SEQUENCER_CONTROLLER_LEDS__SHARP 2
#define STEP_SEQUENCER_CONTROLLER_LEDS_G 13
#define STEP_SEQUENCER_CONTROLLER_LEDS_G_SHARP 3
#define STEP_SEQUENCER_CONTROLLER_LEDS_A 14
#define STEP_SEQUENCER_CONTROLLER_LEDS_A_SHARP 4
#define STEP_SEQUENCER_CONTROLLER_LEDS_B 15
#define STEP_SEQUENCER_CONTROLLER_LEDS_C2 16
#define STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_DOWN 17
#define STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP 18
#define STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT 19
#define STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE 20
#define STEP_SEQUENCER_CONTROLLER_LEDS_BACK 21
#define STEP_SEQUENCER_CONTROLLER_LEDS_NEXT 22

#include "stdint.h"
#include "Step.h"

namespace developmentKit::stepSequencer
{
    class Controller
    {
    public:
        void Init(uint32_t);
        void SetKeyState(uint32_t keys);
        void Process(uint32_t currentProcessTimeUs);
        bool GetGate();
        bool GetAccent();
        bool GetSlide();
        bool GetPreviousSlide();
        uint8_t GetNote();
        void SetStepTime(uint32_t newStepTimeUs);
        uint64_t GetLedState();
        void SetTempo(uint8_t newTempo);

        // For testing only
        uint8_t GetCurrentStepIndex() { return currentStepIndex; }
        uint8_t GetMode() { return mode; }
        void SetSteps(Step newSteps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT]);
        Step *GetSteps() { return steps; }
        uint32_t GetLastTicks() { return lastTicks; }
        void SetLastTicks(uint32_t newLastTicks) { lastTicks = newLastTicks; }

    private:
        Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT];
        uint8_t currentStepIndex;
        uint8_t mode;
        uint32_t keyState;
        uint32_t stepTimeUs;
        uint32_t gateTimeUs;
        uint32_t lastTicks;
        uint32_t ticksPerUs;
        uint8_t tempo;
        bool gate;
        bool ledStates[STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS];
        const uint8_t noteToLedLookup[STEP_SEQUENCER_CONTROLLER_NUMBER_OF_NOTE_KEYS] = {9, 0, 10, 1, 11, 12, 2, 13, 3, 14, 4, 15, 16};
        void UpdateLedStates();
        uint8_t GetNoteFromKeyPressed(uint8_t keyPressed);
        void ActivateCurrentStep();
        void OnPlayPressed();
        void OnRecordPressed();
        void OnBackPressed();
        void OnNextPressed();
        void OnOctaveDownPressed();
        void OnOctaveUpPressed();
        void OnAccentPressed();
        void OnSlidePressed();
        void OnNoteKeyPressed(uint8_t keyIndex);
        void CheckForKeyPressEvent();
        void CheckForClockEvent(uint32_t currentProcessTimeUs);
    };
}

#endif