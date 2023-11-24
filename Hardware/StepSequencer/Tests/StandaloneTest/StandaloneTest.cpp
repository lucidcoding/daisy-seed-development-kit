#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/mcp23x17.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed hardware;
Mcp23017 mcp;
bool ledOn = true;

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
    //mcp.PortMode(MCPPort::A, 0xFF, 0xFF); // Inputs, pullup
    
    mcp.PortMode(MCPPort::A, 0x00); // Outputs
    mcp.PortMode(MCPPort::B, 0x00); // Outputs

    mcp.PinMode(8, MCPMode::OUTPUT, false);
    mcp.PinMode(9, MCPMode::INPUT, false);
    //mcp.PinMode(0, MCPMode::INPUT_PULLUP, false);
    //mcp.PinMode(1, MCPMode::INPUT_PULLUP, false);
    //mcp.PinMode(14, MCPMode::OUTPUT, false);
    mcp.WritePin(8, 1);
    //mcp.PinMode(15, 0);

    uint8_t countdown = 8;
    while (1)
    {
        /*mcp.Read();
        uint8_t inp = mcp.GetPin(0);
        hardware.PrintLine("Read: %d", inp);
        mcp.WritePin(8, inp);
        System::Delay(1000);*/

        //mcp.WritePin(8, ledOn);
        ledOn = !ledOn;
        mcp.WritePin(0, ledOn);
        mcp.Read();
        uint8_t inp = mcp.GetPin(9);
        hardware.PrintLine("LedOn: %d, inp: %d", ledOn, inp);
        System::Delay(1000);

        /*mcp.WritePin(8, ledOn);
        mcp.WritePin(9, 1);
        mcp.WritePin(12, 1);
        mcp.WritePin(13, 0);
        mcp.WritePin(14, 0);
        mcp.WritePin(15, 0);
        System::Delay(1);
        mcp.WritePin(8, ledOn);
        mcp.WritePin(9, 1);
        mcp.WritePin(12, 0);
        mcp.WritePin(13, 1);
        mcp.WritePin(14, 0);
        mcp.WritePin(15, 0);
        System::Delay(1);
        mcp.WritePin(8, ledOn);
        mcp.WritePin(9, 1);
        mcp.WritePin(12, 0);
        mcp.WritePin(13, 0);
        mcp.WritePin(14, 1);
        mcp.WritePin(15, 0);
        System::Delay(1);
        mcp.WritePin(8, ledOn);
        mcp.WritePin(9, 1);
        mcp.WritePin(12, 0);
        mcp.WritePin(13, 0);
        mcp.WritePin(14, 0);
        mcp.WritePin(15, 1);
        System::Delay(1);*/


        /*hardware.PrintLine("Output High");
        mcp.PinMode(15, MCPMode::OUTPUT, false);
        mcp.WritePin(15, 1);
        mcp.Read();
        uint8_t inp = mcp.GetPin(0);
        uint8_t inp2 = mcp.GetPin(1);
        hardware.PrintLine("Read: %d, %d", inp, inp2);
        System::Delay(1000);
        hardware.PrintLine("Output Low");
        mcp.PinMode(15, MCPMode::OUTPUT, false);
        mcp.WritePin(15, 0);
        mcp.Read();
        inp = mcp.GetPin(0);
        inp2 = mcp.GetPin(1);
        hardware.PrintLine("Read: %d, %d", inp, inp2);
        System::Delay(1000);
        hardware.PrintLine("Input");
        mcp.PinMode(15, MCPMode::INPUT, false);
        mcp.Read();
        inp = mcp.GetPin(0);
        inp2 = mcp.GetPin(1);
        hardware.PrintLine("Read: %d, %d", inp, inp2);
        System::Delay(1000);*/

    }
}