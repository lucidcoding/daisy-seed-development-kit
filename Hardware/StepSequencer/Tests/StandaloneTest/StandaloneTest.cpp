#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed hardware;
Mcp23017 mcp;
bool ledOn = true;
bool stableState[24];
bool lastState[24];
uint32_t lastDebounceTime[24];
uint32_t debounceDelay = 1000;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");

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

    const uint8_t columnPins[6] = {8, 9, 10, 11, 12, 13};
    const uint8_t inputPins[4] = {0, 2, 3, 7};

    const uint8_t switchLookup[6][4] = {
        {10, 11, 22, 0},
        {9, 12, 21, 1},
        {8, 13, 20, 2},
        {7, 14, 19, 3},
        {6, 15, 18, 4},
        {5, 16, 17, 255}};

    while (1)
    {
        const uint32_t waitTime = 50;

        for (uint8_t columnPinIndex = 0; columnPinIndex < 6; columnPinIndex++)
        {
            uint8_t columnPin = columnPins[columnPinIndex];

            for (uint8_t columnPinToSetIndex = 0; columnPinToSetIndex < 6; columnPinToSetIndex++)
            {
                uint8_t columnPinToSet = columnPins[columnPinToSetIndex];
                mcp.WritePin(columnPinToSet, columnPin == columnPinToSet ? 0 : 1);
            }

            uint16_t read = mcp.Read();
            uint8_t inputPortReading = read & 0xFF;

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

                    if ((System::GetUs() - lastDebounceTime[switchIndex]) > debounceDelay)
                    {
                        if (currentState != stableState[switchIndex])
                        {
                            stableState[switchIndex] = currentState;
                            lastState[switchIndex] = currentState;

                            if (stableState[switchIndex] == true)
                            {
                                hardware.PrintLine("Switch ON %d", switchIndex);
                            }
                            else
                            {
                                hardware.PrintLine("Switch OFF %d", switchIndex);
                            }
                        }
                    }

                    lastState[switchIndex] = currentState;
                }
            }

            System::DelayUs(waitTime);
        }
    }
}