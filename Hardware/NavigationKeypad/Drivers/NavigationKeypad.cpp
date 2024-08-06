#include "NavigationKeypad.h"

namespace developmentKit::hardware::navigationKeypad::drivers
{
    void NavigationKeypad::Init()
    {
        Mcp23017::Config config;
        config.transport_config.i2c_address = NAVIGATION_KEYPAD_I2C_ADDRESS;
        config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
        config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
        config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
        config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};
        config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
        mcp.Init(config);
        mcp.PortMode(MCPPort::A, 0xFF, 0xFF, 0xFF);
        mcp.PortMode(MCPPort::B, 0xFF, 0xFF, 0xFF);

        for (uint8_t keyIndex = 0; keyIndex < 16; keyIndex++)
        {
            debounceBuffer[keyIndex] = (uint8_t)0;
        }

        lastState = NAVIGATION_KEYPAD_NO_KEY_PRESS;
    }

    uint8_t NavigationKeypad::ScanKeys()
    {
        uint8_t returnState = NAVIGATION_KEYPAD_NO_KEY_PRESS;
        uint8_t newState = lastState;

        mcp.Read();

        for (uint8_t keyIndex = 0; keyIndex < NAVIGATION_KEYPAD_NUMBER_OF_KEYS; keyIndex++)
        {
            uint8_t currentIndividualState = mcp.GetPin(keyIndex) > 0 ? 1 : 0;
            debounceBuffer[keyIndex] = (debounceBuffer[keyIndex] << 1) | currentIndividualState;

            if (debounceBuffer[keyIndex] == 0x7F)
            {
                newState |= (0x01 << keyIndex);
            }

            if (debounceBuffer[keyIndex] == 0xFE)
            {
                newState &= ~(1 << keyIndex);
            }
        }

        if (lastState != newState)
        {
            returnState = newState;
        }

        lastState = newState;
        return returnState;
    }
}