#include "Keys.h"

namespace developmentKit::stepSequencer
{
    void Keys::Init()
    {
        Mcp23017::Config config;
        config.transport_config.i2c_address = 0x20;
        config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
        config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
        config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
        config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};
        config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
        mcp.Init(config);
        mcp.PortMode(MCPPort::A, 0xFF, 0xFF);
        mcp.PortMode(MCPPort::B, 0x00);
        mcp.WritePort(MCPPort::B, 0xFF);
        lastKeyState = 0;
    }

    uint32_t Keys::ScanNextColumn(uint32_t currentProcessTimeUs)
    {
        uint32_t returnValue = STEP_SEQUENCER_KEYS_NO_KEY_PRESS;
        uint8_t columnPin = columnPins[columnPinIndex];
        mcp.WritePort(MCPPort::B, ~(0x01 << (columnPin - 8)));
        mcp.Read();

        for (uint8_t inputPinIndex = 0; inputPinIndex < 4; inputPinIndex++)
        {
            uint8_t switchIndex = switchLookup[columnPinIndex][inputPinIndex];

            if (switchIndex != STEP_SEQUENCER_KEYS_NO_KEY_PRESS) 
            {
                uint8_t inputPin = inputPins[inputPinIndex];
                uint8_t currentIndividualState = mcp.GetPin(inputPin) == 255 ? 0 : 1;
                uint8_t lastIndivdualState = (lastKeyState & (1 << switchIndex)) > 0 ? 1 : 0;

                if (currentIndividualState != lastIndivdualState)
                {
                    lastDebounceTime[switchIndex] = currentProcessTimeUs;
                }

                if ((currentProcessTimeUs - lastDebounceTime[switchIndex]) > STEP_SEQUENCER_KEYS_DEBOUNCE_TIME)
                {
                    uint8_t stableIndividualState = (stableKeyState & (1 << switchIndex)) > 0 ? 1 : 0;
                    if (currentIndividualState != stableIndividualState)
                    {
                        if (currentIndividualState == 1)
                        {
                            stableKeyState = stableKeyState | (1 << switchIndex);
                        }
                        else
                        {
                            stableKeyState = stableKeyState & ~(1 << switchIndex);
                        }

                        returnValue = stableKeyState;
                    }
                }

                if (currentIndividualState == 1)
                {
                    lastKeyState = lastKeyState | (1 << switchIndex);
                }
                else
                {
                    lastKeyState = lastKeyState & ~(1 << switchIndex);
                }
            }
        }

        columnPinIndex = (columnPinIndex + 1) % 6;
        return returnValue;
    }
}