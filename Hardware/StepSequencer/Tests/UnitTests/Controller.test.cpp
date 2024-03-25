#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../../Drivers/Debug.h"
#include "catch.hpp"
#include "../../Drivers/Controller.h"
#include "../../Drivers/Step.h"
#include "stdint.h"
#include "MockHardware.h"

using namespace developmentKit::hardware::stepSequencer::drivers;
using namespace developmentKit::hardware::stepSequencer::tests::unitTests;

uint32_t currentTicks;
MockHardware mockHardware;
Controller controller;

void Setup()
{
    currentTicks = 0;
    controller.Init(1);
    mockHardware.Clear();
    controller.SetHardware(&mockHardware);
    controller.EnterTestMode();
}

Step *GetGatedSteps()
{
    static Step steps[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT];
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
    static Step steps[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT];

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
    static Step steps[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT];

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
    static Step steps[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT];
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
    static Step steps[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT];
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

void Advance(Controller *controller, uint8_t ticks)
{
    for (uint8_t i = 0; i < ticks; i++)
    {
        controller->Process(currentTicks);
        currentTicks++;
    }
}

TEST_CASE("Pressing Play sets mode to play")
{
    Setup();
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    Step *steps = GetGatedSteps();
    controller.SetSteps(steps);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
}

TEST_CASE("Calling Process() without pressing play does not advance step")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    uint8_t currentStepIndex = controller.GetCurrentStepIndex();
    REQUIRE(currentStepIndex == 0);
    Advance(&controller, 9);
    REQUIRE(currentStepIndex == 0);
}

TEST_CASE("Pressing play advances to first step")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    Advance(&controller, 8);
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    Advance(&controller, 1);
    REQUIRE(controller.GetCurrentStepIndex() == 1);
}

TEST_CASE("Correct notes are played at each step")
{
    const uint8_t expectedNotes[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT * STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6,
        7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8,
        9, 9, 9, 9, 9, 9, 9, 9,
        10, 10, 10, 10, 10, 10, 10, 10,
        11, 11, 11, 11, 11, 11, 11, 11,
        12, 12, 12, 12, 12, 12, 12, 12,
        13, 13, 13, 13, 13, 13, 13, 13,
        14, 14, 14, 14, 14, 14, 14, 14,
        15, 15, 15, 15, 15, 15, 15, 15};

    Setup();
    controller.SetSteps(GetIncrementingNoteSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);

    for (uint16_t i = 0; i < (STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT * STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP); i++)
    {
        Advance(&controller, 1);

        DYNAMIC_SECTION("Checking note for " << i)
        {
            REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == expectedNotes[i]);
        }
    }
}

TEST_CASE("Correct gate is registered at each tick")
{
    const bool expectedGates[STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT * STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP] = {
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0};

    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);

    for (uint16_t i = 0; i < (STEP_SEQUENCER_CONTROLLER_TEST_STEP_COUNT * STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP); i++)
    {
        Advance(&controller, 1);

        DYNAMIC_SECTION("Checking gate for " << i)
        {
            REQUIRE(controller.GetGate() == expectedGates[i]);
        }
    }
}

TEST_CASE("Pressing stop mid gate closes gate after current has finished")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    Advance(&controller, 3);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);

    for (uint16_t i = 0; i < (3 * STEP_SEQUENCER_CONTROLLER_TEST_TICKS_PER_STEP); i++)
    {
        Advance(&controller, 1);
        REQUIRE(!controller.GetGate());
    }
}

TEST_CASE("Pressing stop after gate keeps gate closed")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    Advance(&controller, 6);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);

    for (uint16_t i = 0; i < 30; i++)
    {
        Advance(&controller, 1);
        REQUIRE(!controller.GetGate());
    }
}

TEST_CASE("Pressing stop mid long gate closes gate immediately")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    Advance(&controller, 22);
    REQUIRE(controller.GetGate());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);

    for (uint16_t i = 0; i < 8; i++)
    {
        Advance(&controller, 1);
        REQUIRE(!controller.GetGate());
    }
}

TEST_CASE("Pressing record in stop mode puts it into step record mode")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
}

TEST_CASE("Pressing record in play mode puts it into step record mode")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
}

TEST_CASE("Entering notes in step record mode sets steps correctly")
{
    Setup();
    controller.SetSteps(GetClearedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_B);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_A);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);

    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_G);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_F);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);

    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_E);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_D);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);

    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);

    Step *expectedSteps = GetExpectedProgrammedSteps();
    Step *actualSteps = controller.GetSteps();

    for (uint16_t i = 0; i < 16; i++)
    {
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
    Setup();
    controller.SetSteps(GetIncrementingNoteSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 0);
    Advance(&controller, 9);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 1);
}

TEST_CASE("Tick count resets when pressing play after a few ticks")
{
    Setup();
    controller.SetSteps(GetIncrementingNoteSteps());
    Advance(&controller, 6);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    Advance(&controller, 1);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 0);
    Advance(&controller, 7);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 0);
    Advance(&controller, 1);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 1);
}

TEST_CASE("Only LEDs for first step shown when started")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.Process(0);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing PLAY turns PLAY LED on")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_PLAY) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing play and advancing 2 steps turns play LED on and correct steps")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    Advance(&controller, 17);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_G) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_PLAY) | ((uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 2));
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing PLAY twice turns PLAY LED off")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0 | ((uint64_t)(uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC turns REC LED on")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC and advancing 2 steps turns correct LEDs on")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_G) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC) | ((uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 2));
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC and advancing to a step with no gate does not show note LED")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    controller.SetKeyState(0);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    controller.SetKeyState(0);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC) | ((uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 1));
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("GetPreviousSlide returns false if on first step and last step is not slide")
{
    Setup();
    controller.SetSteps(GetClearedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    REQUIRE(controller.GetPreviousSlide() == false);
}

TEST_CASE("GetPreviousSlide returns false if current step is slide but previous is not, and true if previous is")
{
    Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);
    Advance(&controller, 17);
    REQUIRE(controller.GetPreviousSlide() == false);
    Advance(&controller, 8);
    REQUIRE(controller.GetPreviousSlide() == true);
}

TEST_CASE("Pressing FUNC + D# leaves seqSyncSource the same")
{
    uint64_t expectedLedStates = (uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1;
    Setup();
    controller.SetSteps(GetClearedSteps());
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL);
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP));
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL);
    REQUIRE(controller.GetLedState() == expectedLedStates);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL);
    REQUIRE(controller.GetLedState() == expectedLedStates);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL);
    expectedLedStates = ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(controller.GetLedState() == expectedLedStates);
}

TEST_CASE("Pressing FUNC + D# 2 times changes seqSyncSource to InternalSequencerPulseSync")
{
    uint64_t expectedLedStates = (uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 1);
    Setup();
    controller.SetSteps(GetClearedSteps());
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL);
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP));
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC);
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP));
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_PULSE);
    REQUIRE(controller.GetLedState() == expectedLedStates);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_PULSE);
    expectedLedStates = ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(controller.GetLedState() == expectedLedStates);
}

TEST_CASE("Pressing FUNC + D# 3 times changes seqSyncSource to InternalSequencerMidiSync")
{
    uint64_t expectedLedStates = (uint64_t)1 << (STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1 + 2);
    Setup();
    controller.SetSteps(GetClearedSteps());
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_INTERNAL);
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP));
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC);
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP));
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC);
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP));
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_SETTING_SEQ_SYNC);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SYNC);
    REQUIRE(controller.GetLedState() == expectedLedStates);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    REQUIRE(controller.GetSeqSyncSource() == STEP_SEQUENCER_CONTROLLER_SEQ_SYNC_MIDI_SYNC);
    expectedLedStates = ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(controller.GetLedState() == expectedLedStates);
}

void RequireLedsFlash(uint64_t ledsToFlash)
{
    REQUIRE(controller.GetLedState() == ledsToFlash);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == 0);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == ledsToFlash);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == 0);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == ledsToFlash);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == 0);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == ledsToFlash);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == 0);
    Advance(&controller, 8);
    REQUIRE(controller.GetLedState() == ledsToFlash);
    Advance(&controller, 8);
}

TEST_CASE("Pressing FUNC + C2 clears the pattern")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2));
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_BLINK);
    RequireLedsFlash(0x1FFFF);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    Step *actualSteps = controller.GetSteps();

    for (uint8_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with index: " << stepIndex)
        {
            REQUIRE(actualSteps[stepIndex].note == 0);
            REQUIRE(actualSteps[stepIndex].gate == true);
            REQUIRE(actualSteps[stepIndex].octaveDown == false);
            REQUIRE(actualSteps[stepIndex].octaveUp == false);
            REQUIRE(actualSteps[stepIndex].slide == false);
            REQUIRE(actualSteps[stepIndex].accent == false);
        }
    }
}

TEST_CASE("Pressing FUNC + PATTERN followed by a whole note key saves the pattern")
{
    Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_FUNC) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN));
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_SAVE);
    REQUIRE(controller.GetLedState() == 0x1AB5);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_B);
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_BLINK);
    RequireLedsFlash(1 << STEP_SEQUENCER_CONTROLLER_KEYS_B);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    REQUIRE(mockHardware.savePatternsCount == 1);
    Step *actualSteps = controller.GetSavedPatterns();
    Step *actualHardwareSteps = mockHardware.savedPatterns;

    for (uint16_t patternIndex = 0; patternIndex < 6; patternIndex++)
    {
        for (uint16_t stepIndex = 0; stepIndex < 16; stepIndex++)
        {
            uint16_t savedPatternIndex = (patternIndex * 16) + stepIndex;

            DYNAMIC_SECTION("Checking step with patternIndex " << savedPatternIndex << ", p:" << patternIndex << ", s:" << stepIndex)
            {
                REQUIRE(actualSteps[savedPatternIndex].note == 0);
                REQUIRE(actualSteps[savedPatternIndex].gate == true);
                REQUIRE(actualSteps[savedPatternIndex].octaveDown == false);
                REQUIRE(actualSteps[savedPatternIndex].octaveUp == false);
                REQUIRE(actualSteps[savedPatternIndex].slide == false);
                REQUIRE(actualSteps[savedPatternIndex].accent == false);

                REQUIRE(actualHardwareSteps[savedPatternIndex].note == 0);
                REQUIRE(actualHardwareSteps[savedPatternIndex].gate == true);
                REQUIRE(actualHardwareSteps[savedPatternIndex].octaveDown == false);
                REQUIRE(actualHardwareSteps[savedPatternIndex].octaveUp == false);
                REQUIRE(actualHardwareSteps[savedPatternIndex].slide == false);
                REQUIRE(actualHardwareSteps[savedPatternIndex].accent == false);
            }
        }
    }

    Step *expectedSteps = GetVariedSteps();

    for (uint16_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with patternIndex 6 and spepIndex: " << stepIndex)
        {
            REQUIRE(actualSteps[96 + stepIndex].note == expectedSteps[stepIndex].note);
            REQUIRE(actualSteps[96 + stepIndex].gate == expectedSteps[stepIndex].gate);
            REQUIRE(actualSteps[96 + stepIndex].octaveDown == expectedSteps[stepIndex].octaveDown);
            REQUIRE(actualSteps[96 + stepIndex].octaveUp == expectedSteps[stepIndex].octaveUp);
            REQUIRE(actualSteps[96 + stepIndex].slide == expectedSteps[stepIndex].slide);
            REQUIRE(actualSteps[96 + stepIndex].accent == expectedSteps[stepIndex].accent);

            REQUIRE(actualHardwareSteps[96 + stepIndex].note == expectedSteps[stepIndex].note);
            REQUIRE(actualHardwareSteps[96 + stepIndex].gate == expectedSteps[stepIndex].gate);
            REQUIRE(actualHardwareSteps[96 + stepIndex].octaveDown == expectedSteps[stepIndex].octaveDown);
            REQUIRE(actualHardwareSteps[96 + stepIndex].octaveUp == expectedSteps[stepIndex].octaveUp);
            REQUIRE(actualHardwareSteps[96 + stepIndex].slide == expectedSteps[stepIndex].slide);
            REQUIRE(actualHardwareSteps[96 + stepIndex].accent == expectedSteps[stepIndex].accent);
        }
    }

    for (uint16_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with patternIndex 7 and spepIndex: " << stepIndex)
        {
            REQUIRE(actualSteps[112 + stepIndex].note == 0);
            REQUIRE(actualSteps[112 + stepIndex].gate == true);
            REQUIRE(actualSteps[112 + stepIndex].octaveDown == false);
            REQUIRE(actualSteps[112 + stepIndex].octaveUp == false);
            REQUIRE(actualSteps[112 + stepIndex].slide == false);
            REQUIRE(actualSteps[112 + stepIndex].accent == false);

            REQUIRE(actualHardwareSteps[112 + stepIndex].note == 0);
            REQUIRE(actualHardwareSteps[112 + stepIndex].gate == true);
            REQUIRE(actualHardwareSteps[112 + stepIndex].octaveDown == false);
            REQUIRE(actualHardwareSteps[112 + stepIndex].octaveUp == false);
            REQUIRE(actualHardwareSteps[112 + stepIndex].slide == false);
            REQUIRE(actualHardwareSteps[112 + stepIndex].accent == false);
        }
    }
}

TEST_CASE("Given state is stopped, pressing PATTERN followed by a whole note key recalls the pattern")
{
    Setup();
    controller.SetSteps(GetClearedSteps());

    // Set pattern 3 to VariedSteps
    Step *variedSteps = GetVariedSteps();
    Step *stepsToSet = controller.GetSavedPatterns();
    uint8_t patternIndex = 1;

    for (uint8_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        stepsToSet[patternIndex * 16 + stepIndex].note = variedSteps[stepIndex].note;
        stepsToSet[patternIndex * 16 + stepIndex].gate = variedSteps[stepIndex].gate;
        stepsToSet[patternIndex * 16 + stepIndex].octaveDown = variedSteps[stepIndex].octaveDown;
        stepsToSet[patternIndex * 16 + stepIndex].octaveUp = variedSteps[stepIndex].octaveUp;
        stepsToSet[patternIndex * 16 + stepIndex].accent = variedSteps[stepIndex].accent;
        stepsToSet[patternIndex * 16 + stepIndex].slide = variedSteps[stepIndex].slide;
    }

    // Check loaded steps are clear
    Step *actualSteps = controller.GetSteps();

    for (uint8_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with stepIndex: " << stepIndex)
        {
            REQUIRE(actualSteps[stepIndex].note == 0);
            REQUIRE(actualSteps[stepIndex].gate == true);
            REQUIRE(actualSteps[stepIndex].octaveDown == false);
            REQUIRE(actualSteps[stepIndex].octaveUp == false);
            REQUIRE(actualSteps[stepIndex].slide == false);
            REQUIRE(actualSteps[stepIndex].accent == false);
        }
    }

    // Press PATTERN - should so into LOAD state and display LEDs for patterns to select
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_LOAD);
    REQUIRE(controller.GetLedState() == 0x1AB5);

    // Then press the note key and release - this should light up the respective LED and sets the selected pattern
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_D));
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN);
    REQUIRE(controller.GetLedState() == (1 << STEP_SEQUENCER_CONTROLLER_LEDS_D));

    // Releasing PATTERN sends it back to STOP state and copies the selected pattern
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    uint64_t expectedLedStates = 0x00 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(controller.GetLedState() == expectedLedStates);
    actualSteps = controller.GetSteps();

    for (uint16_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with stepIndex: " << stepIndex)
        {
            REQUIRE(actualSteps[stepIndex].note == variedSteps[stepIndex].note);
            REQUIRE(actualSteps[stepIndex].gate == variedSteps[stepIndex].gate);
            REQUIRE(actualSteps[stepIndex].octaveDown == variedSteps[stepIndex].octaveDown);
            REQUIRE(actualSteps[stepIndex].octaveUp == variedSteps[stepIndex].octaveUp);
            REQUIRE(actualSteps[stepIndex].accent == variedSteps[stepIndex].accent);
            REQUIRE(actualSteps[stepIndex].slide == variedSteps[stepIndex].slide);
        }
    }
}

/*TEST_CASE("Given state is playing, pressing PATTERN followed by a whole note key recalls the pattern")
{
    Setup();
    controller.SetSteps(GetClearedSteps());

    // Press PLAY
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    controller.SetKeyState(0);

    // Set pattern 3 to VariedSteps
    Step *variedSteps = GetVariedSteps();
    Step *stepsToSet = controller.GetSavedPatterns();
    uint8_t patternIndex = 3;

    for (uint8_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        stepsToSet[patternIndex * 16 + stepIndex].note = variedSteps[stepIndex].note;
        stepsToSet[patternIndex * 16 + stepIndex].gate = variedSteps[stepIndex].gate;
        stepsToSet[patternIndex * 16 + stepIndex].octaveDown = variedSteps[stepIndex].octaveDown;
        stepsToSet[patternIndex * 16 + stepIndex].octaveUp = variedSteps[stepIndex].octaveUp;
        stepsToSet[patternIndex * 16 + stepIndex].accent = variedSteps[stepIndex].accent;
        stepsToSet[patternIndex * 16 + stepIndex].slide = variedSteps[stepIndex].slide;
    }

    // Check loaded steps are clear
    Step *actualSteps = controller.GetSteps();

    for (uint8_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with stepIndex: " << stepIndex)
        {
            REQUIRE(actualSteps[stepIndex].note == 0);
            REQUIRE(actualSteps[stepIndex].gate == true);
            REQUIRE(actualSteps[stepIndex].octaveDown == false);
            REQUIRE(actualSteps[stepIndex].octaveUp == false);
            REQUIRE(actualSteps[stepIndex].slide == false);
            REQUIRE(actualSteps[stepIndex].accent == false);
        }
    }

    // Press PATTERN - should go into PLAY_SELECT_PATERN state and display LEDs for patterns to select
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY_SELECT_PATTERN);
    REQUIRE(controller.GetLedState() == 0x1AB5);

    // Then press the note key and release - this should light up the respective LED and sets the selected pattern
    // GOES INTO ANOTHER STATE?
    controller.SetKeyState((1 << STEP_SEQUENCER_CONTROLLER_KEYS_PATTERN) | (1 << STEP_SEQUENCER_CONTROLLER_KEYS_F));
    controller.SetKeyState(0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY_SELECTING_PATTERN);
    REQUIRE(controller.GetLedState() == (1 << STEP_SEQUENCER_CONTROLLER_LEDS_F));

    // Releasing PATTERN sends it back to PLAY state state and copies the selected pattern and continues playing
    // NO LONGER RESETTING TO ZERO
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
    uint64_t expectedLedStates = 0x00 | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | ((uint64_t)1 << STEP_SEQUENCER_CONTROLLER_LEDS_STEP_1);
    REQUIRE(controller.GetLedState() == expectedLedStates);

    for (uint16_t stepIndex = 0; stepIndex < 16; stepIndex++)
    {
        DYNAMIC_SECTION("Checking step with stepIndex: " << stepIndex)
        {
            REQUIRE(actualSteps[112 + stepIndex].note == variedSteps[stepIndex].note);
            REQUIRE(actualSteps[112 + stepIndex].gate == variedSteps[stepIndex].gate);
            REQUIRE(actualSteps[112 + stepIndex].octaveDown == variedSteps[stepIndex].octaveDown);
            REQUIRE(actualSteps[112 + stepIndex].octaveUp == variedSteps[stepIndex].octaveUp);
            REQUIRE(actualSteps[112 + stepIndex].slide == variedSteps[stepIndex].accent);
            REQUIRE(actualSteps[112 + stepIndex].accent == variedSteps[stepIndex].slide);
        }
    }
}*/