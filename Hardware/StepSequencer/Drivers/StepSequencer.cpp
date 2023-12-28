#include "StepSequencer.h"
#include "IKeys.h"
#include "ILeds.h"
#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

    StepSequencer::StepSequencer(IKeys *keys, ILeds* leds)
    {
        this->keys = keys;
        this->leds = leds;
    }

    void StepSequencer::Init()
    {
        stepCount = 16;
        currentStep = 0;
        stepInterval = 250;
        tick = stepInterval - 1;
        mode = STEP_SEQUENCER_MODE_STOP;
        keys->Init();
        leds->Init();

        steps[0].accent = true;
        steps[1].note = 1;
        steps[3].note = 4;
        steps[3].accent = true;
        steps[5].octaveUp = true;
        steps[6].note = 1;
        steps[6].slide = true;
        steps[8].note = 10;
        steps[10].note = 9;
        steps[10].accent = true;
        steps[12].note = 3;
        steps[12].octaveDown = true;
        steps[13].note = 3;
        steps[14].note = 1;
        steps[15].octaveUp = true;
    }

    void StepSequencer::UpdateLedsForCurrentStep()
    {
        Step step = steps[currentStep];

        for (uint8_t ledToSet = 0; ledToSet <= 15; ledToSet++)
        {
            leds->SetLed(ledToSet, ledToSet == noteToLedLookup[step.note]);
        }

        leds->SetLed(17, step.octaveDown);
        leds->SetLed(18, step.octaveUp);
        leds->SetLed(19, step.accent);
        leds->SetLed(20, step.slide);
    }

    void StepSequencer::StartStepRecording()
    {
        mode = STEP_SEQUENCER_MODE_STEP_REC;
        currentStep = 0;
        UpdateLedsForCurrentStep();
    }

    void StepSequencer::Process(uint32_t currentProcessTimeUs)
    {
        if (currentProcessTimeUs - lastProcessTimeUs > STEP_SEQUENCER_PROCESS_INTERVAL_US)
        {
            lastProcessTimeUs = currentProcessTimeUs;

            if (mode == STEP_SEQUENCER_MODE_PLAY && tick++ >= stepInterval)
            {
                tick = 0;

                UpdateLedsForCurrentStep();
                currentStep = (currentStep + 1) % stepCount;
            }

            uint64_t retVal = keys->ScanNextColumn();

            if (retVal < 255)
            {
                leds->SetLed(retVal, true);

                if (retVal == 6)
                {
                    mode = (mode == STEP_SEQUENCER_MODE_PLAY) ? STEP_SEQUENCER_MODE_STOP : STEP_SEQUENCER_MODE_PLAY;
                }

                if (retVal == 7)
                {
                    StartStepRecording();
                }

                if (mode == STEP_SEQUENCER_MODE_STEP_REC)
                {
                    if (retVal == 21)
                    {
                        if (currentStep > 0)
                        {
                            currentStep--;
                            UpdateLedsForCurrentStep();
                        }
                    }

                    if (retVal == 22)
                    {
                        if (currentStep < (stepCount - 1))
                        {
                            currentStep++;
                            UpdateLedsForCurrentStep();
                        }
                    }
                }

                if (mode == STEP_SEQUENCER_MODE_STEP_REC)
                {
                    if (std::find(std::begin(noteToLedLookup), std::end(noteToLedLookup), retVal) != std::end(noteToLedLookup))
                    {
                        uint8_t note = std::distance(noteToLedLookup, std::find(noteToLedLookup, noteToLedLookup + 16, retVal));
                        steps[currentStep].note = note;
                    }

                    if (retVal == 17)
                    {
                        steps[currentStep].octaveDown = !steps[currentStep].octaveDown;
                    }

                    if (retVal == 18)
                    {
                        steps[currentStep].octaveUp = !steps[currentStep].octaveUp;
                    }

                    if (retVal == 19)
                    {
                        steps[currentStep].accent = !steps[currentStep].accent;
                    }

                    if (retVal == 20)
                    {
                        steps[currentStep].slide = !steps[currentStep].slide;
                    }

                    UpdateLedsForCurrentStep();
                }
            }

            leds->ScanNextColumn();
        }
    }
}