#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../../Drivers/Debug.h"
#include "catch.hpp"
#include "../../Drivers/SequencerBrain.h"
#include "../../Drivers/Step.h"
#include "stdint.h"

using namespace developmentKit::stepSequencer;

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

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

    steps[11].note = 0;
    steps[11].gate = true;

    steps[12].gate = false;
    steps[13].gate = false;
    steps[14].gate = false;
    steps[15].gate = false;
}

/*SCENARIO("Pressing Play sets mode to play")
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

SCENARIO("Calling Process() without pressing play does not advance step")
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

SCENARIO("Pressing play advances to first step")
{
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    sequencerBrain.SetSteps(steps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);

    for (uint8_t i = 0; i < 9; i++)
    {
        sequencerBrain.Process(0);
    }

    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);
    sequencerBrain.Process(0);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 1);
}

SCENARIO("Pressing play switches on gate for specified time")
{
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    sequencerBrain.SetSteps(steps);
    REQUIRE(!sequencerBrain.GetGate());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    REQUIRE(!sequencerBrain.GetGate());

    for (uint8_t i = 0; i < 4; i++)
    {
        sequencerBrain.Process(0);
    }

    REQUIRE(sequencerBrain.GetGate());
    sequencerBrain.Process(0);
    REQUIRE(!sequencerBrain.GetGate());
}*/

SCENARIO("Correct notes are played necessarily in the right order")
{
    DEBUG("TEST 1");
    std::cout << "TEST 2";
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
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

    sequencerBrain.SetSteps(incrementingSteps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);

    for (uint8_t i = 0; i < 16; i++)
    {
        DEBUG("1: expecting: " << (uint16_t)i << ". getting: " << (uint16_t)sequencerBrain.GetCurrentStep().note);
        REQUIRE(sequencerBrain.GetCurrentStep().note == i);

        for (uint8_t j = 0; j < 9; j++)
        {
            sequencerBrain.Process(0);
        }

        DEBUG("2: expecting: " << (uint16_t)i << ". getting: " << (uint16_t)sequencerBrain.GetCurrentStep().note);
        REQUIRE(sequencerBrain.GetCurrentStep().note == i);
        sequencerBrain.Process(0);
    }
}
