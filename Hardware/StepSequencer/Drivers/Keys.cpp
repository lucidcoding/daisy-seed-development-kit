#include "Keys.h"
#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

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
    }

    uint64_t Keys::Process()
    {
        uint64_t returnValue = 0;
        uint8_t columnPin = columnPins[columnPinIndex];
        mcp.WritePort(MCPPort::B, ~(0x01 << (columnPin - 8)));
        mcp.Read();

        for (uint8_t inputPinIndex = 0; inputPinIndex < 4; inputPinIndex++)
        {
            uint8_t switchIndex = switchLookup[columnPinIndex][inputPinIndex];

            if (switchIndex != 255) // Unused
            {
                uint8_t inputPin = inputPins[inputPinIndex];
                bool currentState = mcp.GetPin(inputPin) == 255 ? false : true;

                if (currentState != lastState[switchIndex])
                {
                    lastDebounceTime[switchIndex] = System::GetUs();
                }

                if ((System::GetUs() - lastDebounceTime[switchIndex]) > STEP_SEQUENCER_DEBOUNCE_TIME)
                {
                    if (currentState != stableState[switchIndex])
                    {
                        stableState[switchIndex] = currentState;
                        lastState[switchIndex] = currentState;

                        if (stableState[switchIndex] == true)
                        {
                            returnValue |= (1 << switchIndex);
                        }
                        else
                        {
                            // Handle switch off here.
                        }
                    }
                }

                lastState[switchIndex] = currentState;
            }
        }

        columnPinIndex = (columnPinIndex + 1) % 6;
        return returnValue;
    }
}