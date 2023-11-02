#include "daisysp.h"
#include "daisy_seed.h"
#include "Hardware/PotentiometerArray/PotentiometerArray.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit;
MidiUartHandler midi;

static DaisySeed  hardware;
Oscillator osc;
Adsr adsr;
bool gate;
PotentiometerArray potentiometerArray;

AnalogControl knob;
Parameter param;
//AnalogControl knobs[1];

/*
void ProcessAnalogControls()
{
    for(size_t i = 0; i < 1; i++)
    {
        knobs[i].Process();
    }
}*/

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
	//ProcessAnalogControls();
	//knob.Process();
	potentiometerArray.Process();


	//
    float osc_out, env_out;
    for(size_t i = 0; i < size; i += 2)
    {
		env_out = adsr.Process(gate);
        osc.SetAmp(env_out / 10);
        osc_out = osc.Process();

        out[i]  = osc_out;
        out[i + 1] = osc_out;
    }
}

int midiTick = 0;

void InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}


// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
	switch (m.type)
	{
	case NoteOn:
	{
		hardware.SetLed(true);

		NoteOnEvent p = m.AsNoteOn();
            
		// This is to avoid Max/MSP Note outs for now..
        if(m.data[1] != 0)
        {
            p = m.AsNoteOn();
			gate = true;
            osc.SetFreq(mtof(p.note));
            //osc.SetAmp((p.velocity / 127.0f));
        }

		break;
	}
	case NoteOff:
	{
		hardware.SetLed(false);
		gate = false;
        //osc.SetAmp(0);
		break;
	}
	default:
		break;
	}

	switch (m.srt_type)
	{
	case TimingClock:
	{

		midiTick = (midiTick + 1) % 24;

		if (midiTick == 0)
		{
			hardware.SetLed(true);
		}

		if (midiTick == 12)
		{
			hardware.SetLed(false);
		}

		break;
	}
	default:
		break;
	}

}

int main(void) 
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    
	hardware.Configure();
    hardware.Init();
	
	//InitMidi();

	hardware.StartLog(false);
	//InitMidi();

        System::Delay(250);


	// Init knobs
	

    // Synthesis
    /*sample_rate = seed.AudioSampleRate();
    adsr.Init(sample_rate);
    osc.Init(sample_rate);
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
	osc.SetAmp(0.1);*/
	
	
    /*osc.Init(sample_rate);
	

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);*/

	
    //Set envelope parameters
   /* adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.25);*/

 	//AdcChannelConfig adcConfig;

    // Initialize it to pin D1 as an OUTPUT
    potentiometerArray.seed = &hardware;



	//potentiometerArray.adcConfig = &adcConfig;
	potentiometerArray.Init();
	

    //adcConfig.InitSingle(seed.GetPin(19));
    //seed.adc.Init(&adcConfig, 1);


	/*knob.Init(seed.adc.GetPtr(0), seed.AudioCallbackRate());

	param.Init(knob, 0, 1.0f, Parameter::LINEAR);*/
    //seed.adc.Init(&adcConfig, 1);
    hardware.adc.Start();



    hardware.StartAudio(AudioCallback);




	//hw.StartLog();
	/*while (1)
	{
		midi.Listen();

		while (midi.HasEvents())
		{
			HandleMidiMessage(midi.PopEvent());
		}
	}*/



	while(1)
	{
		
		//potentiometerArray.Process();
		potentiometerArray.Poll();
		///param.Process();
		//float val = param.Process();
		//float val = knob.GetRawFloat();
		//float val = seed.adc.GetFloat(0);
		//int val3 = val * 10000;
		//uint16_t val3 = seed.adc.Get(0);
		//sprintf(str, "val:%d", val3);
		//seed.PrintLine(str);
	}

}
