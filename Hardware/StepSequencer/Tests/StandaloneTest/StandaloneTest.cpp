#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"
#include "../../Drivers/StepSequencerKeys.h";
#include "../../Drivers/StepSequencerLeds.h";

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;

static DaisySeed hardware;
Mcp23017 mcp;
Mcp23017 mcp2;
bool ledOn = true;
bool stableState[24];
bool lastState[24];
uint32_t lastDebounceTime[24];
uint32_t debounceDelay = 1000;
StepSequencerKeys keys;
StepSequencerLeds leds;

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

    /*Mcp23017::Config config2;
    config2.transport_config.i2c_address = 0x21;
    config2.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    config2.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
    config2.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
    config2.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};
    config2.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
    mcp2.Init(config2);
    mcp2.PortMode(MCPPort::A, 0x00);
    mcp2.PortMode(MCPPort::B, 0x00);
    mcp2.WritePort(MCPPort::A, 0x00);
    mcp2.WritePort(MCPPort::B, 0x00);*/

    keys.Init();
    leds.Init();
    leds.SetLed(0, true);
    leds.SetLed(4, true);
    leds.SetLed(9, true);
    leds.SetLed(14, true);
    leds.SetLed(19, true);

    while (1)
    {

        uint64_t retVal = keys.Process();

        if (retVal > 0)
        {
            hardware.PrintLine("RetVal: %d", retVal);
        }

        leds.Process();

        System::DelayUs(500);
        //System::Delay(1000);
    }
}