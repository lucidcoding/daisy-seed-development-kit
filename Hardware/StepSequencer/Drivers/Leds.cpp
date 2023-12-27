#include "Leds.h"
#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::stepSequencer
{
    using namespace daisysp;
    using namespace daisy;

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
    }

    void Leds::SetLed(uint8_t ledIndex, bool state)
    {
        ledStates[ledIndex] = state;
        /*if(state)
        {
            ledStates |= (ledIndex << 1);
        }
        else
        {
            ledStates &= ~(ledIndex << 1);
        }*/
    }

    void Leds::Process()
    {
        mcp.WritePort(MCPPort::A, 0x00);
        uint8_t currentColumnPin = columnPins[currentColumnIndex];
        mcp.WritePort(MCPPort::B, ~(0x01 << (currentColumnPin - 8)));

        uint8_t portAValue = 0x00 << currentColumnPin;

        for (uint8_t currentRowIndex = 0; currentRowIndex < 4; currentRowIndex++)
        {
            uint8_t currentRowPin = rowPins[currentRowIndex];

            uint8_t ledIndex = ledLookup[currentColumnIndex][currentRowIndex];
            uint8_t state = 0;

            if (ledIndex != 255 && ledStates[ledIndex])
            {
                portAValue = portAValue | (0x01 << currentRowPin);
            }
        }

        
        mcp.WritePort(MCPPort::A, portAValue);
        currentColumnIndex = (currentColumnIndex + 1) % 6;
    }
}