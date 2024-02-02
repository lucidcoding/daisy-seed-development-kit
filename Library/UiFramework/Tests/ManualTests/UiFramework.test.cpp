#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "../../Display.h"

#define PIN_I2C_SCL     8
#define PIN_I2C_SDA     9
#define PIN_ENC_A       30
#define PIN_ENC_CLICK   0
#define PIN_ENC_B       29

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework;

static DaisySeed hardware;
OledDisplay<SSD130xI2c128x64Driver> hardwareDisplay;
Parameters parameters;
Display display = Display(&hardwareDisplay, &parameters);

Encoder encoder;
Oscillator osc;
Adsr adsr;
Metro tick;
bool gate;

void ProcessEncoder();

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    ProcessEncoder();
    float level = parameters.level;

    float osc_out, env_out;
    for(size_t i = 0; i < size; i += 2)
    {        // When the metro ticks, trigger the envelope to start.
        if(tick.Process())
        {
            gate = !gate;
        }

        // Use envelope to control the amplitude of the oscillator.
        env_out = adsr.Process(gate);
        osc.SetAmp(env_out * parameters.level);
        osc_out = osc.Process();

        out[i]  = osc_out;
        out[i + 1] = osc_out;
    }
}

void ProcessEncoder()
{
	encoder.Debounce();
	int encoderIncrement = encoder.Increment();

	if (encoderIncrement == 1)
	{
		display.Increment();
	}

	if (encoderIncrement == -1)
	{
		display.Decrement();
	}

	if (encoder.RisingEdge())
	{
		display.Select();
	}
}
 
void UpdateDisplay()
{
	display.Paint();
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    adsr.Init(sampleRate);
    osc.Init(sampleRate);

    // Set up metro to pulse every second
    tick.Init(1.0f, sampleRate);

    //Set envelope parameters
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .05);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);

    adsr.SetSustainLevel(.01);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(220);
    osc.SetAmp(0.25);


    hardware.StartAudio(AudioCallback);
    hardware.StartLog(false);
    hardware.PrintLine("Starting...");

    OledDisplay<SSD130xI2c128x64Driver>::Config disp_cfg;
    disp_cfg.driver_config.transport_config.i2c_address = 0x3C;
	disp_cfg.driver_config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
	disp_cfg.driver_config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ; 
	disp_cfg.driver_config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
	disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, PIN_I2C_SCL};
	disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, PIN_I2C_SDA};
	hardwareDisplay.Init(disp_cfg);

    encoder.Init(hardware.GetPin(PIN_ENC_A),
        hardware.GetPin(PIN_ENC_B),
        hardware.GetPin(PIN_ENC_CLICK),
        sampleRate);

    /*hardwareDisplay.Fill(false);
    hardwareDisplay.SetCursor(1, 1);
    hardwareDisplay.WriteString("Test 1", Font_7x10, true);
    hardwareDisplay.Update();*/
    //display.Paint();

    while (1)
    {
        UpdateDisplay();
        hardware.PrintLine("parameters.level:%3.5f", parameters.level);
        System::Delay(1000);
    }
}