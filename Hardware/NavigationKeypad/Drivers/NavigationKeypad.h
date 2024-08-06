#pragma once
#ifndef NAVIGATION_KEYPAD_H
#define NAVIGATION_KEYPAD_H

#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"

#define NAVIGATION_KEYPAD_I2C_ADDRESS 0x22
#define NAVIGATION_KEYPAD_SCAN_INTERVAL_US 350
#define NAVIGATION_KEYPAD_NUMBER_OF_KEYS 8
#define NAVIGATION_KEYPAD_NO_KEY_PRESS 0
#define NAVIGATION_KEYPAD_KEY_UP 0b00000001
#define NAVIGATION_KEYPAD_KEY_LEFT 0b00000010
#define NAVIGATION_KEYPAD_KEY_ENTER 0b00000100
#define NAVIGATION_KEYPAD_KEY_DOWN 0b00001000
#define NAVIGATION_KEYPAD_KEY_RIGHT 0b00010000
#define NAVIGATION_KEYPAD_KEY_BACK 0b00100000
#define NAVIGATION_KEYPAD_KEY_MENU 0b01000000
#define NAVIGATION_KEYPAD_KEY_FUNC 0b10000000

namespace developmentKit::hardware::navigationKeypad::drivers
{
    using namespace daisysp;
    using namespace daisy;

    class NavigationKeypad
    {
    public:
        void Init();
        uint8_t ScanKeys();

    private:
        Mcp23017 mcp;
        uint8_t lastState;
        uint8_t debounceBuffer[NAVIGATION_KEYPAD_NUMBER_OF_KEYS];
    };
}

#endif