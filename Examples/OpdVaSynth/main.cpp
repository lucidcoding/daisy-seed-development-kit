#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
//#include "UserInterface.h"
//#include "../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"
#include "vasynth.h"

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9
#define PIN_ENC_A 30
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 29

using namespace daisysp;
using namespace daisy;
/*using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::utilities;
using namespace developmentKit::library::uiFramework::tabs::view;
using namespace developmentKit::hardware::potentiometerArray::drivers;*/

static DaisySeed hardware;
Encoder encoder;
static Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;
VASynth vasynth;
OpdModSources modSources;
uint8_t gPlay = PLAY_ON;
/*PotentiometerArray potentiometerArray;
UserInterface userInterface;

UiParameter
    levelParameter,
    noteParameter,
    attackParameter,
    decayParameter,
    sustainParameter,
    releaseParameter;

void UpdateDisplay()
{
    userInterface.Paint();
}

void ProcessEncoder()
{
    encoder.Debounce();
    int encoderIncrement = encoder.Increment();

    if (encoderIncrement == 1)
    {
        userInterface.Increment();
    }

    if (encoderIncrement == -1)
    {
        userInterface.Decrement();
    }

    if (encoder.RisingEdge())
    {
        userInterface.Select();
    }
}

void ProcessPotentiometerArray()
{
    potentiometerArray.Process();
    float values[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        values[i] = potentiometerArray.analogControl[i].GetRawFloat();
    }

    userInterface.SetPotentiometerValues(values);
}*/

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
float voice_left, voice_right;
	/*hardware.encoder.Debounce();
    inc = hardware.encoder.Increment();
    if (hardware.encoder.RisingEdge())
	{
			slot ++;
			if (slot == 16)
			{
				slot = 0;
			}
	}
	if (hardware.encoder.Increment())
	{
		if(inc > 0)
		{
			vasynth.FlashLoad(slot);		
		}
		else if(inc < 0)
		{
			vasynth.FlashSave(slot);
		}
		
	}
	
	
	hardware.button1.Debounce();
	hardware.button2.Debounce();
	if(hardware.button1.RisingEdge())
	{
		mode = ((mode / 8) + 1) * 8;
	}
	
	if(hardware.button2.RisingEdge())
	{	
		mode ++;			
	}
	
	#ifdef OPD_MEASURE
	// measure - start
	DWT->CYCCNT = 0;
	#endif
	
	// audio
	Controls();*/
	for (size_t n = 0; n < size; n += 2)
	{	
		if (gPlay == PLAY_ON)
		{
			// voices
			
			vasynth.Process(&voice_left, &voice_right);
			
			
			if (vasynth.input_channel_ == INPUT_CHANNEL_NONE)
			{
				out[n] = voice_left;
				out[n + 1] = voice_right;
			} else {
				out[n] = voice_left + in[n];
				out[n + 1] = voice_right + in[n + 1];		
			}
		} else {
			out[n] = 0;
			out[n + 1] = 0;		
		}		
	}
	


	#ifdef OPD_MEASURE
	// measure - stop
	if (DWT->CYCCNT > 390000)
	{
		hardware.SetLed(true);
	}
	#endif
}

/*void InitOscillator(float sampleRate)
{
    oscillator.Init(sampleRate);
    oscillator.SetWaveform(Oscillator::WAVE_SQUARE);
    oscillator.SetAmp(0.125f);
    oscillator.SetFreq(mtof(64));
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, 0.0f);
    adsr.SetTime(ADSR_SEG_DECAY, 0.125f);
    adsr.SetTime(ADSR_SEG_RELEASE, 0.125f);
    adsr.SetSustainLevel(.01);
}

void InitMetro(float sampleRate)
{
    metro.Init(1.0f, sampleRate);
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitEncoder(float sampleRate)
{
    encoder.Init(
        hardware.GetPin(PIN_ENC_A),
        hardware.GetPin(PIN_ENC_B),
        hardware.GetPin(PIN_ENC_CLICK),
        sampleRate);
}

void InitDisplay()
{
    userInterface.Init();
}*/

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.adc.Start();
    hardware.StartLog(false);
    float sampleRate = hardware.AudioSampleRate();
    /*InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitPotentiometerArray();
    InitMetro(sampleRate);
    InitEncoder(sampleRate);
    InitDisplay();*/
    vasynth.First();

    /*
    ]MidiUartHandler::Config midi_config;
	midi.Init(midi_config);
    */
   
    hardware.StartAudio(AudioCallback);

    //UpdateDisplay();

    uint32_t lastTicksRefresh = System::GetTick();
    uint32_t lastTicksRefreshDisplay = System::GetTick();
    uint32_t lastTicksRShowValues = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;
            //UpdateDisplay();
        }

        /*if (currentTicks - lastTicksRShowValues > (1000000 * ticksPerUs))
        {
            lastTicksRShowValues = currentTicks;
            hardware.PrintLine("P0: %3.5f\tP1: %3.5f\tP2: %3.5f\tP3: %3.5f\t",
                               potentiometerArray.analogControl[0].GetRawFloat(),
                               potentiometerArray.analogControl[1].GetRawFloat(),
                               potentiometerArray.analogControl[2].GetRawFloat(),
                               potentiometerArray.analogControl[3].GetRawFloat());
        }*/
    }
}