#include "Leds.h"

namespace developmentKit::hardware::stepSequencer::drivers
{
    void Leds::Init()
    {
        Mcp23017::Config config;
        config.transport_config.i2c_address = 0x21;
        config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
        config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
        config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
        config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};
        config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
        mcp.Init(config);
        mcp.PortMode(MCPPort::A, 0x00);
        mcp.PortMode(MCPPort::B, 0x00);
        mcp.WritePort(MCPPort::A, 0x00);
        mcp.WritePort(MCPPort::B, 0x00);
        ticksPerUs = System::GetTickFreq() / 1000000;
    }

    void Leds::Scan(uint64_t newState, uint32_t currentTicks)
    {
        state = newState;

        if (currentTicks - lastTicks > (STEP_SEQUENCER_LEDS_SCAN_INTERVAL_US * ticksPerUs))
        {
            lastTicks = currentTicks;
            mcp.WritePort(MCPPort::A, 0);
            uint8_t currentColumnPin = columnPins[currentColumnIndex];
            mcp.WritePort(MCPPort::B, ~(1 << (currentColumnPin - 8)));
            uint8_t portAValue = 0 << currentColumnPin;

            for (uint8_t currentRowIndex = 0; currentRowIndex < STEP_SEQUENCER_LEDS_NUMBER_OF_ROWS; currentRowIndex++)
            {
                uint8_t currentRowPin = rowPins[currentRowIndex];
                uint8_t ledIndex = ledLookup[currentColumnIndex][currentRowIndex];
                uint8_t ledState = (state >> ledIndex) & 1;

                if (ledIndex != STEP_SEQUENCER_LEDS_NOT_USED && ledState == 1)
                {
                    portAValue = portAValue | (1 << currentRowPin);
                }
            }

            mcp.WritePort(MCPPort::A, portAValue);
            currentColumnIndex = (currentColumnIndex + 1) % STEP_SEQUENCER_LEDS_NUMBER_OF_COLUMNS;
        }
    }
}