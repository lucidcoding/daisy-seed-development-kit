#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "../../Drivers/Debug.h"
#include "catch.hpp"
#include "../../Drivers/Controller.h"
#include "../../Drivers/Step.h"
#include "stdint.h"

using namespace developmentKit::stepSequencer;

Controller Setup()
{
    Controller controller;
    controller.Init();
    controller.SetTicksPerStep(10);
    return controller;
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

void Advance(Controller *controller, uint8_t steps)
{
    for (uint8_t i = 0; i < steps; i++)
    {
        controller->Process();
    }
}

TEST_CASE("Pressing Play sets mode to play")
{
    Controller controller;
    controller.Init();
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    controller.SetTicksPerStep(10);
    Step *steps = GetGatedSteps();
    controller.SetSteps(steps);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STOP);
    Advance(&controller, 1);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
}

TEST_CASE("Calling Process() without pressing play does not advance step")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    uint8_t currentStepIndex = controller.GetCurrentStepIndex();
    REQUIRE(currentStepIndex == 0);
    Advance(&controller, 11);
    REQUIRE(currentStepIndex == 0);
}

TEST_CASE("Pressing play advances to first step")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    Advance(&controller, 10);
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    Advance(&controller, 1);
    REQUIRE(controller.GetCurrentStepIndex() == 1);
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

    Controller controller = Setup();
    controller.SetSteps(GetIncrementingNoteSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);

    for (uint16_t i = 0; i < 160; i++)
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

    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);

    for (uint16_t i = 0; i < 160; i++)
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
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 3);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 2);
    REQUIRE(controller.GetGate());

    for (uint16_t i = 0; i < 30; i++)
    {
        Advance(&controller, 1);
        REQUIRE(!controller.GetGate());
    }
}

TEST_CASE("Pressing stop after gate keeps gate closed")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 6);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);

    for (uint16_t i = 0; i < 30; i++)
    {
        Advance(&controller, 1);
        REQUIRE(!controller.GetGate());
    }
}

TEST_CASE("Pressing stop mid long gate closes gate immediately")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 28);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 2);

    for (uint16_t i = 0; i < 20; i++)
    {
        Advance(&controller, 1);
        REQUIRE(!controller.GetGate());
    }
}

TEST_CASE("Pressing record in stop mode puts it into step record mode")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
}

TEST_CASE("Pressing record in play mode puts it into step record mode")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_STEP_REC);
}

TEST_CASE("Entering notes in step record mode sets steps correctly")
{
    Controller controller = Setup();
    controller.SetSteps(GetClearedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C2);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_B);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_A_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_A);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);

    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_G_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_G);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_F_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_F);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_UP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);

    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_E);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_D_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_OCTAVE_DOWN);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_ACCENT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_SLIDE);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_D);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);

    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C_SHARP);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_C);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);

    Step *expectedSteps = GetExpectedProgrammedSteps();
    Step *actualSteps = controller.GetSteps();

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
    Controller controller = Setup();
    controller.SetSteps(GetIncrementingNoteSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 1);
    REQUIRE(controller.GetMode() == STEP_SEQUENCER_CONTROLLER_MODE_PLAY);
    REQUIRE(controller.GetCurrentStepIndex() == 0);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 0);
    Advance(&controller, 10);
    REQUIRE(controller.GetSteps()[controller.GetCurrentStepIndex()].note == 1);
}

TEST_CASE("Only LEDs for first step shown when started")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing PLAY turns PLAY LED on")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 1);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_PLAY);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing play and advancing 2 steps turns play LED on and correct steps")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 21);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_G) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_PLAY);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing PLAY twice turns PLAY LED off")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 1);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC turns REC LED on")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_C) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_OCTAVE_UP) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC and advancing 2 steps turns correct LEDs on")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_G) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_ACCENT) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_SLIDE) | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("Pressing REC and advancing to a step with no gate does not show note LED")
{
    Controller controller = Setup();
    controller.SetSteps(GetVariedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_REC);
    Advance(&controller, 1);
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_NEXT);
    Advance(&controller, 1);
    uint64_t actualLedStates = controller.GetLedState();
    uint64_t expectedLedStates = 0x00 | (1 << STEP_SEQUENCER_CONTROLLER_LEDS_REC);
    REQUIRE(actualLedStates == expectedLedStates);
}

TEST_CASE("GetPreviousSlide returns false if on first step and last step is not slide")
{
    Controller controller = Setup();
    controller.SetSteps(GetClearedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 1);
    REQUIRE(controller.GetPreviousSlide() == false);
}

TEST_CASE("GetPreviousSlide returns false if current step is slide but previous is not, and true if previous is")
{
    Controller controller = Setup();
    controller.SetSteps(GetGatedSteps());
    controller.SetKeyState(1 << STEP_SEQUENCER_CONTROLLER_KEYS_PLAY);
    Advance(&controller, 21);
    REQUIRE(controller.GetPreviousSlide() == false);
    Advance(&controller, 10);
    REQUIRE(controller.GetPreviousSlide() == true);
}