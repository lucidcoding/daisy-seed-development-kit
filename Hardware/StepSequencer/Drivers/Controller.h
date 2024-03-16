#pragma once
#ifndef STEP_SEQUENCER_CONTROLLER_H
#define STEP_SEQUENCER_CONTROLLER_H
#define STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT 16

#define STEP_SEQUENCER_CONTROLLER_MODE_STOP 0
#define STEP_SEQUENCER_CONTROLLER_MODE_PLAY 1
#define STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC 2
#define STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC 3
#define STEP_SEQUENCER_CONTROLLER_MODE_CLEARING 4

#define STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS 35
#define STEP_SEQUENCER_CONTROLLER_NUMBER_OF_NOTE_KEYS 13
#define STEP_SEQUENCER_CONTROLLER_NUMBER_OF_KEYS 23
#define STEP_SEQUENCER_CONTROLLER_NO_KEY_PRESS 255
#define STEP_SEQUENCER_CONTROLLER_NOT_NOTE_KEY 255

#define STEP_SEQUENCER_CONTROLLER_KEYS_FUNC 20
#define STEP_SEQUENCER_CONTROLLER_KEYS_PLAY 17
#define STEP_SEQUENCER_CONTROLLER_KEYS_REC 18
#define STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN 19
#define STEP_SEQUENCER_CONTROLLER_KEYS_C 0
#define STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP 1
#define STEP_SEQUENCER_CONTROLLER_KEYS_D 2
#define STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP 3
#define STEP_SEQUENCER_CONTROLLER_KEYS_E 4
#define STEP_SEQUENCER_CONTROLLER_KEYS_F 5
#define STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP 6
#define STEP_SEQUENCER_CONTROLLER_KEYS_G 7
#define STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP 8
#define STEP_SEQUENCER_CONTROLLER_KEYS_A 9
#define STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP 10
#define STEP_SEQUENCER_CONTROLLER_KEYS_B 11
#define STEP_SEQUENCER_CONTROLLER_KEYS_C2 12
#define STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN 13
#define STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP 15
#define STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT 14
#define STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE 16
#define STEP_SEQUENCER_CONTROLLER_KEYS_BACK 21
#define STEP_SEQUENCER_CONTROLLER_KEYS_NEXT 22

#define STEP_SEQUENCER_CONTROLLER_LEDS_PLAY 33
#define STEP_SEQUENCER_CONTROLLER_LEDS_REC 34
#define STEP_SEQUENCER_CONTROLLER_LEDS_C 0
#define STEP_SEQUENCER_CONTROLLER_LEDS_C_SHARP 1
#define STEP_SEQUENCER_CONTROLLER_LEDS_D 2
#define STEP_SEQUENCER_CONTROLLER_LEDS_D_SHARP 3
#define STEP_SEQUENCER_CONTROLLER_LEDS_E 4
#define STEP_SEQUENCER_CONTROLLER_LEDS_F 5
#define STEP_SEQUENCER_CONTROLLER_LEDS__SHARP 6
#define STEP_SEQUENCER_CONTROLLER_LEDS_G 7
#define STEP_SEQUENCER_CONTROLLER_LEDS_G_SHARP 8
#define STEP_SEQUENCER_CONTROLLER_LEDS_A 9
#define STEP_SEQUENCER_CONTROLLER_LEDS_A_SHARP 10
#define STEP_SEQUENCER_CONTROLLER_LEDS_B 11
#define STEP_SEQUENCER_CONTROLLER_LEDS_C2 12
#define STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_DOWN 13
#define STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT 14
#define STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP 15
#define STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE 16
#define STEP_SEQUENCER_CONTROLLER_LEDS_BACK 21
#define STEP_SEQUENCER_CONTROLLER_LEDS_NEXT 22
#define STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 17

#include "stdint.h"
#include "Step.h"

//#include "daisy_seed.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    class Controller
    {
    public:
        enum SeqSyncSource
        {
            InternalSequencerInternalSync,
            InternalSequencerPulseSync,
            InternalSequencerMidiSync,
            MidiInSequencer
        };
        void Init(uint32_t);
        void SetKeyState(uint32_t);
        void Process(uint32_t);
        bool GetGate();
        bool GetAccent();
        bool GetSlide();
        bool GetPreviousSlide();
        uint8_t GetNote();
        uint64_t GetLedState();
        void SetTempo(uint8_t);

        // For testing only
        void SetStepTime(uint32_t);
        uint8_t GetCurrentStepIndex() { return currentStepIndex; }
        uint8_t GetMode() { return mode; }
        void SetSteps(Step newSteps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT]);
        Step *GetSteps() { return steps; }
        uint32_t GetLastTicks() { return lastStepStartTicks; }
        void SetLastTicks(uint32_t newLastTicks) { lastStepStartTicks = newLastTicks; }
        SeqSyncSource GetSeqSyncSource() { return seqSyncSource; }
        void SetBlinkTimeUs(uint32_t newBlinkTimeUs) { blinkTimeUs = newBlinkTimeUs; }
        //daisy::DaisySeed *daisy;

    private:
        Step steps[STEP_SEQUENCER_CONTROLLER_DEFAULT_STEP_COUNT];
        uint8_t currentStepIndex;
        uint8_t mode;
        uint32_t stepTimeUs;
        uint32_t gateTimeUs;
        uint32_t ticksPerUs;
        uint32_t lastStepStartTicks;
        uint8_t tempo;
        bool gate;
        bool playJustPressed;
        uint32_t blinkTimeUs;
        uint32_t lastBlinkTicks;
        uint8_t blinkCount;
        bool blinkOn;
        bool blinkJustStarted;
        bool ledStates[STEP_SEQUENCER_CONTROLLER_NUMBER_OF_LEDS];
        uint32_t lastKeyState;
        SeqSyncSource seqSyncSource;
        void ClearSteps();
        void UpdateLedStates();
        uint8_t GetNoteFromKeyPressed(uint32_t);
        void ActivateCurrentStep();
        void OnSeqSyncSelectPressed();
        void OnClearPressed();
        void OnPlayPressed();
        void OnRecordPressed();
        void OnBackPressed();
        void OnNextPressed();
        void OnOctaveDownPressed();
        void OnOctaveUpPressed();
        void OnAccentPressed();
        void OnSlidePressed();
        void OnNoteKeyPressed(uint64_t);
        void OnFunctionKeyReleased();
        void OnKeyPressed(uint32_t);
        void OnKeyReleased(uint32_t);
        void CheckForClockEvent(uint32_t);
    };
}

#endif