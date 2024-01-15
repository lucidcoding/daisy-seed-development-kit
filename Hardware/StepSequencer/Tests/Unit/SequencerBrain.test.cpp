#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../../Drivers/Debug.h"
#include "catch.hpp"
#include "../../Drivers/SequencerBrain.h"
#include "../../Drivers/Step.h"
#include "stdint.h"

using namespace developmentKit::stepSequencer;

SequencerBrain Setup()
{
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    sequencerBrain.SetStepInterval(10);
    return sequencerBrain;
}

Step *GetGatedSteps()
{
    static Step steps[16];
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
    return steps;
}

Step *GetIncrementingNoteSteps()
{
    static Step steps[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        steps[i].note = i;
        steps[i].gate = true;
        steps[i].octaveDown = false;
        steps[i].octaveUp = false;
        steps[i].accent = false;
        steps[i].slide = false;
    }

    return steps;
}

Step *GetClearedSteps()
{
    static Step steps[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        steps[i].note = 0;
        steps[i].gate = true;
        steps[i].octaveDown = false;
        steps[i].octaveUp = false;
        steps[i].accent = false;
        steps[i].slide = false;
    }

    return steps;
}

Step *GetExpectedProgrammedSteps()
{
    static Step steps[16];
    steps[0].note = 12;
    steps[0].gate = true;
    steps[1].note = 11;
    steps[1].gate = true;
    steps[2].note = 10;
    steps[2].gate = true;
    steps[3].note = 9;
    steps[3].gate = true;
    steps[4].note = 8;
    steps[4].gate = true;
    steps[4].slide = true;
    steps[5].note = 7;
    steps[5].gate = true;
    steps[6].note = 6;
    steps[6].gate = true;
    steps[6].octaveDown = true;
    steps[7].note = 5;
    steps[7].gate = true;
    steps[7].octaveUp = true;
    steps[8].note = 4;
    steps[8].gate = true;
    steps[8].accent = true;
    steps[9].note = 3;
    steps[9].gate = true;
    steps[9].octaveDown = true;
    steps[9].accent = true;
    steps[9].slide = true;
    steps[10].note = 2;
    steps[10].gate = true;
    steps[11].gate = false;
    steps[12].note = 1;
    steps[12].gate = true;
    steps[13].note = 1;
    steps[13].gate = false;
    steps[14].note = 0;
    steps[14].gate = true;
    steps[15].gate = false;
    return steps;
}

Step *GetVariedSteps()
{
    static Step steps[16];
    steps[0].note = 0;
    steps[0].gate = true;
    steps[0].octaveUp = true;
    steps[0].accent = false;
    steps[0].slide = false;
    steps[1].gate = false;
    steps[2].note = 7;
    steps[2].gate = true;
    steps[2].slide = true;
    steps[2].accent = true;
    steps[3].note = 0;
    steps[3].gate = true;
    steps[3].accent = true;
    steps[4].note = 3;
    steps[4].gate = true;
    steps[5].note = 0;
    steps[5].gate = true;
    steps[5].octaveDown = true;
    steps[6].gate = false;
    steps[7].note = 7;
    steps[7].gate = true;
    steps[8].note = 3;
    steps[8].gate = true;
    steps[9].gate = false;
    steps[10].note = 0;
    steps[10].gate = true;
    steps[11].note = 1;
    steps[11].gate = true;
    steps[12].gate = false;
    steps[13].gate = false;
    steps[14].gate = false;
    steps[15].gate = false;
    return steps;
}

void Advance(SequencerBrain *sequencerBrain, uint8_t steps)
{
    for (uint8_t i = 0; i < steps; i++)
    {
        sequencerBrain->Process(0);
    }
}

TEST_CASE("Pressing Play sets mode to play")
{
    SequencerBrain sequencerBrain;
    sequencerBrain.Init();
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_STOP);
    sequencerBrain.SetStepInterval(10);
    Step *steps = GetGatedSteps();
    sequencerBrain.SetSteps(steps);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_STOP);
    Advance(&sequencerBrain, 1);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_PLAY);
}

TEST_CASE("Calling Process() without pressing play does not advance step")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    uint8_t currentStepIndex = sequencerBrain.GetCurrentStepIndex();
    REQUIRE(currentStepIndex == 0);
    Advance(&sequencerBrain, 11);
    REQUIRE(currentStepIndex == 0);
}

TEST_CASE("Pressing play advances to first step")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);
    Advance(&sequencerBrain, 10);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);
    Advance(&sequencerBrain, 1);
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

    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetIncrementingNoteSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);

    for (uint16_t i = 0; i < 160; i++)
    {
        Advance(&sequencerBrain, 1);

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

    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);

    for (uint16_t i = 0; i < 160; i++)
    {
        Advance(&sequencerBrain, 1);

        DYNAMIC_SECTION("Checking gate for " << i)
        {
            REQUIRE(sequencerBrain.GetGate() == expectedGates[i]);
        }
    }
}

TEST_CASE("Pressing stop mid gate closes gate after current has finished")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 3);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 2);
    REQUIRE(sequencerBrain.GetGate());

    for (uint16_t i = 0; i < 30; i++)
    {
        Advance(&sequencerBrain, 1);
        REQUIRE(!sequencerBrain.GetGate());
    }
}

TEST_CASE("Pressing stop after gate keeps gate closed")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 6);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);

    for (uint16_t i = 0; i < 30; i++)
    {
        Advance(&sequencerBrain, 1);
        REQUIRE(!sequencerBrain.GetGate());
    }
}

TEST_CASE("Pressing stop mid long gate closes gate immediately")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 28);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 2);

    for (uint16_t i = 0; i < 20; i++)
    {
        Advance(&sequencerBrain, 1);
        REQUIRE(!sequencerBrain.GetGate());
    }
}

TEST_CASE("Pressing record in stop mode puts it into step record mode")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_STEP_REC);
}

TEST_CASE("Pressing record in play mode puts it into step record mode")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetGatedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_STEP_REC);
}

TEST_CASE("Entering notes in step record mode sets steps correctly")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetClearedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_C2);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_B);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_A_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_A);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);

    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_G_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_SLIDE);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_G);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_F_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_OCTAVE_DOWN);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_F);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_OCTAVE_UP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);

    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_E);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_ACCENT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_D_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_OCTAVE_DOWN);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_ACCENT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_SLIDE);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_D);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_C);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);

    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_C_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_C_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_C_SHARP);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_C);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);

    Step *expectedSteps = GetExpectedProgrammedSteps();
    Step *actualSteps = sequencerBrain.GetSteps();

    for (uint16_t i = 0; i < 16; i++)
    {
        //DEBUG("expected: " << (uint16_t)(expectedSteps[i].note) << ", actual: " << (uint16_t)(actualSteps[i].note));
        DYNAMIC_SECTION("Checking step with index: " << i)
        {
            REQUIRE(actualSteps[i].note == expectedSteps[i].note);
            REQUIRE(actualSteps[i].gate == expectedSteps[i].gate);
            REQUIRE(actualSteps[i].octaveDown == expectedSteps[i].octaveDown);
            REQUIRE(actualSteps[i].octaveUp == expectedSteps[i].octaveUp);
            REQUIRE(actualSteps[i].slide == expectedSteps[i].slide);
            REQUIRE(actualSteps[i].accent == expectedSteps[i].accent);
        }
    }
}

TEST_CASE("Pressing play in the middle of step record mode starts playing from begining")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetIncrementingNoteSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 1);
    REQUIRE(sequencerBrain.GetMode() == STEP_SEQUENCER_MODE_PLAY);
    REQUIRE(sequencerBrain.GetCurrentStepIndex() == 0);
    REQUIRE(sequencerBrain.GetCurrentStep().note == 0);
    Advance(&sequencerBrain, 10);
    REQUIRE(sequencerBrain.GetCurrentStep().note == 1);
}

TEST_CASE("Only LEDs for first step shown when started")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_C) | (1 << STEP_SEQUENCER_LEDS_OCTAVE_UP);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing PLAY turns PLAY LED on")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 1);
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_C) | (1 << STEP_SEQUENCER_LEDS_OCTAVE_UP) | (1 << STEP_SEQUENCER_LEDS_PLAY);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing play and advancing 2 steps turns play LED on and correct steps")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 21);
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_G) | (1 << STEP_SEQUENCER_LEDS_ACCENT) | (1 << STEP_SEQUENCER_LEDS_SLIDE) | (1 << STEP_SEQUENCER_LEDS_PLAY);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing PLAY twice turns PLAY LED off")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_PLAY);
    Advance(&sequencerBrain, 1);
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_C) | (1 << STEP_SEQUENCER_LEDS_OCTAVE_UP);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC turns REC LED on")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_C) | (1 << STEP_SEQUENCER_LEDS_OCTAVE_UP) | (1 << STEP_SEQUENCER_LEDS_REC);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC and advancing 2 steps turns correct LEDs on")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_G) | (1 << STEP_SEQUENCER_LEDS_ACCENT) | (1 << STEP_SEQUENCER_LEDS_SLIDE) | (1 << STEP_SEQUENCER_LEDS_REC);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC and advancing to a step with no gate does not show note LED")
{
    SequencerBrain sequencerBrain = Setup();
    sequencerBrain.SetSteps(GetVariedSteps());
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_REC);
    Advance(&sequencerBrain, 1);
    sequencerBrain.SetKeys(STEP_SEQUENCER_KEYS_NEXT);
    Advance(&sequencerBrain, 1);
    uint64_t actualLedStates = sequencerBrain.GetLedStates();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_LEDS_REC);
    REQUIRE(actualLedStates == expectedLedStates);
}
