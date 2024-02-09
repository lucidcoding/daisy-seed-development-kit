#include "Keys.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void Keys::Init()
    {
        Mcp23017::Config config;
        config.transport_config.i2c_address = STEP_SEQUENCER_KEYS_I2C_ADDRESS;
        config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
        config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
        config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
        config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};
        config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
        mcp.Init(config);
        mcp.PortMode(MCPPort::A, 0xFF, 0xFF);
        mcp.PortMode(MCPPort::B, 0x00);
        mcp.WritePort(MCPPort::B, 0xFF);
        state = 0;
        ticksPerUs = System::GetTickFreq() / 1000000;
    }

    uint32_t Keys::ScanNextColumn(uint32_t currentTicks)
    {
        uint32_t returnValue = STEP_SEQUENCER_KEYS_NO_KEY_PRESS;

        if (currentTicks - lastTicks > (STEP_SEQUENCER_KEYS_SCAN_INTERVAL_US * ticksPerUs))
        {
            lastTicks = currentTicks;
            uint8_t columnPin = columnPins[currentColumnIndex];
            mcp.WritePort(MCPPort::B, ~(1 << (columnPin - 8)));
            mcp.Read();

            for (uint8_t rowIndex = 0; rowIndex < STEP_SEQUENCER_KEYS_NUMBER_OF_ROWS; rowIndex++)
            {
                uint8_t keyIndex = keyLookup[currentColumnIndex][rowIndex];

                if (keyIndex != STEP_SEQUENCER_KEYS_NOT_USED)
                {
                    uint8_t rowPin = rowPins[rowIndex];
                    uint8_t currentIndividualState = mcp.GetPin(rowPin) == STEP_SEQUENCER_KEYS_NO_KEY_PRESS ? 0 : 1;
                    uint8_t lastIndivdualState = (state & (1 << keyIndex)) > 0 ? 1 : 0;
                    debounceBuffer[keyIndex] = (debounceBuffer[keyIndex] << 1) | currentIndividualState;

                    if (lastIndivdualState == 0 && debounceBuffer[keyIndex] == 0xFF)
                    {
                        state = state | (1 << keyIndex);
                        returnValue = state;
                    }
                    else if (lastIndivdualState == 1 && debounceBuffer[keyIndex] == 0x00)
                    {
                        state = state & ~(1 << keyIndex);
                        returnValue = state;
                    }
                }
            }

            currentColumnIndex = (currentColumnIndex + 1) % STEP_SEQUENCER_KEYS_NUMBER_OF_COLUMNS;
        }

        return returnValue;
    }
}