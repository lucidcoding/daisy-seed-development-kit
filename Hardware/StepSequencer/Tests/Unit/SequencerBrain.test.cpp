#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../../Drivers/Debug.h"
#include "catch.hpp"
#include "../../Drivers/SequencerBrain.h"
#include "../../Drivers/Step.h"
#include "stdint.h"

using namespace developmentKit::stepSequencer;

Step steps[16];

void SetupSteps()
{
    steps[0].note = 0;
    steps[0].gate = true;

    steps[1].gate = false;

    steps[2].note = 0;
    steps[2].gate = true;
    steps[2].slide = true;

    steps[3].note = 0;
    steps[3].gate = true;

    steps[4].note = 0;
    steps[4].gate = true;

    steps[5].note = 0;
    steps[5].gate = true;

    steps[6].gate = false;

    steps[7].note = 0;
    steps[7].gate = true;

    steps[8].note = 0;
    steps[8].gate = true;

    steps[9].gate = false;

    steps[10].note = 0;
    steps[10].gate = true;

    steps[11].note = 7;
    steps[11].gate = true;

    steps[12].gate = false;
    steps[13].gate = false;
    steps[14].gate = false;
    steps[15].gate = false;
}

TEST_CASE("Pressing Play sets mode to play")
{
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_STOP);
    sequencerBrain.SetStepInterval(10);
    sequencerBrain.SetSteps(steps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_STOP);
    sequencerBrain.Process(0);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_PLAY);
}

TEST_CASE("Calling Process() without pressing play does not advance step")
{
    uint8_t currentStepIndex;
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    sequencerBrain.SetSteps(steps);
    currentStepIndex = sequencerBrain.GetCurrentStepIndex();
    REQUIRE(currentStepIndex == 0);

    for (uint8_t i = 0; i < 11; i++)
    {
        sequencerBrain.Process(0);
    }

    REQUIRE(currentStepIndex == 0);
}

TEST_CASE("Pressing play advances to first step")
{
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    sequencerBrain.SetSteps(steps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);

    for (uint8_t i = 0; i < 10; i++)
    {
        sequencerBrain.Process(0);
    }

    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);
    sequencerBrain.Process(0);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 1);
}

TEST_CASE("Correct notes are played at each step")
{
    const uint8_t expectedNotes[160] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
        11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
        12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
        13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
        14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15};

    Step incrementingSteps[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        incrementingSteps[i].note = i;
        incrementingSteps[i].gate = true;
        incrementingSteps[i].octaveDown = false;
        incrementingSteps[i].octaveUp = false;
        incrementingSteps[i].accent = false;
        incrementingSteps[i].slide = false;
    }

    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    sequencerBrain.SetSteps(incrementingSteps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);

    for (uint16_t i = 0; i < 160; i++)
    {
        sequencerBrain.Process(0);

        DYNAMIC_SECTION("Checking note for " << i)
        {
            REQUIRE(sequencerBrain.GetCurrentStep().note == expectedNotes[i]);
        }
    }
}

TEST_CASE("Correct gate is registered at each tick")
{
    const bool expectedGates[160] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    SetupSteps();
    sequencerBrain.SetSteps(steps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);

    for (uint16_t i = 0; i < 160; i++)
    {
        sequencerBrain.Process(0);

        DYNAMIC_SECTION("Checking gate for " << i)
        {
            REQUIRE(sequencerBrain.GetGate() == expectedGates[i]);
        }
    }
}
