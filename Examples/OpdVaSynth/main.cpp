/*
  
Project: OscPocketD/VASynth - Daisy Pod Version
Description: polyphonic MIDI Virtual Analog synthsizer for OscPcoketD/Base2 (Daisy Seed)
Author: Staffan Melin, staffan.melin@oscillator.se
License: GNU General Public License v3.0
Version: 202109
Project site: http://www.oscillator.se/opensource

EDIT: This code was modified by Steven @moonfriendsynth, who doesn't know shit about shit when it 
comes to C++, but I tried my best. So please be cautious as you read/use this 
code. If you find yourself wondering, "why did he do x, y, or z?" just assume that Steve is saying
"Man, I dunno, good question." I did my best to copy code from either Daisy examples or other open
source projects. Please feel free to take this code and make it better or use it for whatever.

*/

//#define OPD_LOGG // start serial over USB Logger class
//#define OPD_MEASURE // start serial over USB Logger class
#define OPD_BASE_MIDI // version 2 with MIDI, other LCD pins, no CV1 in, no Gate0/1 out

#include "daisy_pod.h"
#include "daisy_seed.h"
#include "daisysp.h"

#include "stm32h7xx_hal.h" // for HAL_NVIC_SystemReset();
#include "core_cm7.h"
#include "dev/lcd_hd44780.h"

#include "main.h"
//#include "qspi.h"
#include "vasynth.h"



using namespace daisy;
using namespace daisysp;
float oldk1, oldk2, k1, k2;
int mode;
// globals
DaisyPod hardware;
extern DaisySeed hw;
MidiUartHandler midi;
float sysSampleRate;
float sysCallbackRate;
static int32_t  inc;
uint8_t slot;
void Controls();
void UpdateKnobs();
void UpdateButtons();
void UpdateLeds();
extern uint8_t preset_max;
extern VASynthSetting preset_setting[PRESET_MAX];

static Parameter transposeParam, cutoffParam, attackParam, releaseParam, detuneParam, portamentoParam;

// hardware

AdcChannelConfig adcConfig[AD_MAX];

// sound + fx
VASynth vasynth;
OpdModSources modSources;
uint8_t gPlay = PLAY_ON;

void ConditionalParameter(float  oldVal,
                          float  newVal,
                          float &param,
						  float  update);

// audio callback

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{	
	float voice_left, voice_right;
	hardware.encoder.Debounce();
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
	Controls();
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



// utility

void RebootToBootloader()
{
	// Initialize Boot Pin
	dsy_gpio_pin bootpin = {DSY_GPIOG, 3};
	dsy_gpio pin;
	pin.mode = DSY_GPIO_MODE_OUTPUT_PP;
	pin.pin = bootpin;
	dsy_gpio_init(&pin);

	// Pull Pin HIGH
	dsy_gpio_write(&pin, 1);

	// wait a few ms for cap to charge
	hardware.DelayMs(10);

	// Software Reset
	HAL_NVIC_SystemReset();
}


#ifdef OPD_BASE_MIDI
// midi handler
void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
			{
				vasynth.NoteOn(p.note, p.velocity);				
			}
	        break;
        }
        case NoteOff:
        {
            NoteOnEvent p = m.AsNoteOn();
            if ((vasynth.midi_channel_ == MIDI_CHANNEL_ALL) || (p.channel == vasynth.midi_channel_))
			{
				vasynth.NoteOff(p.note);
			}			
	        break;
        }        
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            switch(p.control_number)
            {
                case 74: // cutoff, 0-127 -> frequency
                	vasynth.filter_cutoff_ = ((float)p.value / 127.0f) * FILTER_CUTOFF_MAX;
                    vasynth.SetFilter();
                    break;
                case 71: // res, 0-127
                	vasynth.filter_res_ = ((float)p.value / 127.0f);
                    vasynth.SetFilter();
                    break;
                default: break;
            }
            break;
        }
        default: break;
    }
}
#endif



uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float mtoval(uint8_t midiPitch)
{
	float voltsPerNote = 0.0833f; // 1/12 V

	return ((midiPitch * voltsPerNote) / 3.3f);
}



int main(void)
{

	// init hardware
	hardware.Init(true); // true = boost to 480MHz
	hardware.StartAdc();
	sysSampleRate = hardware.AudioSampleRate();
	sysCallbackRate = hardware.AudioCallbackRate();
 
	// init qspi flash for saving and loading patches
	QSPIHandle::Config qspi_config;
	qspi_config.device = QSPIHandle::Config::Device::IS25LP064A;
	qspi_config.mode   = QSPIHandle::Config::Mode::MEMORY_MAPPED;
	qspi_config.pin_config.io0 = {DSY_GPIOF, 8};
	qspi_config.pin_config.io1 = {DSY_GPIOF, 9};
	qspi_config.pin_config.io2 = {DSY_GPIOF, 7};
	qspi_config.pin_config.io3 = {DSY_GPIOF, 6};
	qspi_config.pin_config.clk = {DSY_GPIOF, 10};
	qspi_config.pin_config.ncs = {DSY_GPIOG, 6};
	hw.qspi.Init(qspi_config);
	
	slot = 0;

	// setup incl default values
	vasynth.First();

	// parameters for knob controls
	cutoffParam.Init(hardware.knob1, 30, 30000, cutoffParam.LOGARITHMIC);
	attackParam.Init(hardware.knob1, 0, 20, attackParam.LOGARITHMIC);
	releaseParam.Init(hardware.knob2, 0, 5, releaseParam.LINEAR);
	detuneParam.Init(hardware.knob2, 0.75, 1, detuneParam.LINEAR);
	portamentoParam.Init(hardware.knob2, 0, 1, portamentoParam.LOGARITHMIC);



/*	// init boot/upload button
	Switch buttonUpload;
	buttonUpload.Init(hardware.GetPin(PIN_BUTTON_UPLOAD), 10);
*/	
	#ifdef OPD_BASE_MIDI
	MidiUartHandler::Config midi_config;
	midi.Init(midi_config);
	#endif

	// logging over serial USB
	#ifdef OPD_LOGG
	hardware.StartLog(false); // start log but don't wait for PC - we can be connected to a battery
	#endif

	// let everything settle (esp the LCD)
	System::Delay(100);

	#ifdef OPD_MEASURE
	// setup measurement
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->LAR = 0xC5ACCE55;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	#endif

	// Start calling the audio callback
	hardware.StartAudio(AudioCallback);
	
	

	// Loop forever
	for(;;)
	{
				
		#ifdef OPD_BASE_MIDI
        // handle MIDI Events
        midi.Listen();
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }
		#endif		
	}
}

void ConditionalParameter(float  oldVal,
                          float  newVal,
                          float &param,
						  float  update)
{
    if(abs(oldVal - newVal) < 0.00005)
    {
        param = update;
    }

}


void UpdateButtons()
{
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
	
}

void UpdateLeds()
{
	hardware.UpdateLeds();

}
	
void Controls()
{
	oldk1 = k1;
	oldk2 = k2;

	hardware.ProcessAnalogControls();
	hardware.ProcessDigitalControls();
	UpdateKnobs();
	UpdateLeds();
	UpdateButtons();

}
void UpdateKnobs()
{

	k1 = hardware.knob1.Process();
	k2 = hardware.knob2.Process();	
		 switch(mode)
		{
//---------------Osc1 Waveform and Level ------------------
		case 0:
			hardware.led1.Set(1, 0, 0);
			hardware.led2.Set(0, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				if (k1 < 0.125f)
				{
					vasynth.waveform_ = 0;
				}
					else if (k1 > 0.125f && k1 < 0.25f)
				{
					vasynth.waveform_ = 1;
				}
				else if (k1 > 0.25f && k1 < 0.375f)
				{
					vasynth.waveform_ = 2;
				}
				else if (k1 > 0.375f && k1 < 0.5f)
				{
					vasynth.waveform_ = 3;
				}
				else if (k1 > 0.5f && k1 < 0.625f)
				{
					vasynth.waveform_ = 4;
				}
				else if (k1 > 0.625f && k1 < 0.75f)
				{
					vasynth.waveform_ = 5;
				}
				else if (k1 > 0.75f && k1 < 0.875f)
				{
					vasynth.waveform_ = 6;
				}
				else if (k1 > 0.875f && k1 < 1.0f)
				{
					vasynth.waveform_ = 7;
				}
			
				vasynth.SetWaveform();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.level_ = k2;
			}

            break;
//---------------Osc2 Waveform and Detune ------------------
        case 1:
			hardware.led1.Set(1, 0, 0);
			hardware.led2.Set(1, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				if (k1 < 0.125f)
				{
					vasynth.osc2_waveform_ = 0;
				}
					else if (k1 > 0.125f && k1 < 0.25f)
				{
					vasynth.osc2_waveform_ = 1;
				}
				else if (k1 > 0.25f && k1 < 0.375f)
				{
					vasynth.osc2_waveform_ = 2;
				}
				else if (k1 > 0.375f && k1 < 0.5f)
				{
					vasynth.osc2_waveform_ = 3;
				}
				else if (k1 > 0.5f && k1 < 0.625f)
				{
					vasynth.osc2_waveform_ = 4;
				}
				else if (k1 > 0.625f && k1 < 0.75f)
				{
					vasynth.osc2_waveform_ = 5;
				}
				else if (k1 > 0.75f && k1 < 0.875f)
				{
					vasynth.osc2_waveform_ = 6;
				}
				else if (k1 > 0.875f && k1 < 1.0f)
				{
					vasynth.osc2_waveform_ = 7;
				}
			}
			
			vasynth.SetWaveform();
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.osc2_detune_ = detuneParam.Process();
			}
            break;
//---------------Osc Voices and Portamento ------------------
        case 2:
			hardware.led1.Set(1, 0, 0);
			hardware.led2.Set(0, 1, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				if (k1 < 0.25f)
				{
					vasynth.voices_ = 1;
				}
					else if (k1 > 0.25f && k1 < 0.5f)
				{
					vasynth.voices_ = 3;
				}
				else if (k1 > 0.5f && k1 < 0.75f)
				{
					vasynth.voices_ = 5;
				}
				else if (k1 < 0.75f)
				{
					vasynth.voices_ = 8;
				}
					vasynth.SetWaveform();
					vasynth.SetEG();
					vasynth.SetFilter();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.portamento_ = portamentoParam.Process();
				vasynth.SetWaveform();
			}
            break;
			
//---------------Osc2 Transpose and Level ------------------
		case 3:
			hardware.led1.Set(1, 0, 0);
			hardware.led2.Set(0, 0, 1);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				if (k1 < 0.33)
				{
					vasynth.osc2_transpose_ = -12.0;
				}
				else if (k1 > 0.33 && k1 < 0.66)
				{
					vasynth.osc2_transpose_ = 0.0;
				}
				else if (k1 > 0.66)
				{
					vasynth.osc2_transpose_ = 12.0;
				}
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.osc2_level_ = (k2);
			}
			break;
			
//----------------Pitch Envelope Attack and Release------
		case 4:
			hardware.led1.Set(1, 0, 0);
			hardware.led2.Set(1, 1, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.eg_p_attack_ = attackParam.Process();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.eg_p_release_ = releaseParam.Process();
			}
			vasynth.SetEG();
			break;
			
//-----------------Pitch Envelope Decay and Sustain----------
		case 5:
			hardware.led1.Set(1, 0, 0);
			hardware.led2.Set(0, 1, 1);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.eg_p_decay_ = (k1) * 9;
				vasynth.SetEG();	
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.eg_p_sustain_ = (k2);
				vasynth.SetEG();

			}
			break;
		case 6:
			mode = 0;	
			break;
		case 7:
			mode = 0;
			break;
		//amp
		
//--------------Amp Envelope Attack and Release------
	   case 8:
			hardware.led1.Set(0, 1, 0);
			hardware.led2.Set(0, 0, 0); 
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.eg_a_attack_ = attackParam.Process();
				vasynth.SetEG();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.eg_a_release_ = releaseParam.Process();
				vasynth.SetEG();

			}
            break;
			
			
//--------------Amp Envelope Decay and Sustain-------
        case 9:
			hardware.led1.Set(0, 1, 0);
			hardware.led2.Set(1, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.eg_a_decay_ = (k1) * 9;
				vasynth.SetEG();

			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.eg_a_sustain_ = (k2);
				vasynth.SetEG();

			}
            break;
			
//--------------------Noise and Pan--------------
        case 10:
			hardware.led1.Set(0, 1, 0);
			hardware.led2.Set(0, 1, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.noise_level_ = (k1);
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.pan_ = (k2);
			}
            break;
//----------------Select Presets-------------			
		case 11:
			hardware.led1.Set(0, 1, 0);
			hardware.led2.Set(0, 0, 1);

			if (abs(oldk2 - k2) > 0.0005f )
			{
				if (k2 < 0.125f)
				{
					vasynth.SaveToLive(&preset_setting[0]);
				}
					else if (k2 > 0.125f && k2 < 0.25f)
				{
					vasynth.SaveToLive(&preset_setting[1]);
				}
				else if (k2 > 0.25f && k2 < 0.375f)
				{
					vasynth.SaveToLive(&preset_setting[2]);
				}
				else if (k2 > 0.375f && k2 < 0.5f)
				{
					vasynth.SaveToLive(&preset_setting[3]);
				}
				else if (k2 > 0.5f && k2 < 0.625f)
				{
					vasynth.SaveToLive(&preset_setting[4]);
				}
				else if (k2 > 0.625f && k2 < 0.75f)
				{
					vasynth.SaveToLive(&preset_setting[5]);
				}
				else if (k2 > 0.75f && k2 < 0.875f)
				{
					vasynth.SaveToLive(&preset_setting[5]);
				}
				else if (k2 > 0.875f && k2 < 1.0f)
				{
					vasynth.SaveToLive(&preset_setting[5]);
				}
			}
			break;
		case 12:
			mode = 8;
			break;
		case 13:
			mode = 8;
			break;
		case 14:
			mode = 8;
			break;
		case 15:
			mode = 8;
			break;
		
		//LFO
		
		
//------------------LFO Amp and Frequency------------
		case 16:
			hardware.led1.Set(0, 0, 1);
			hardware.led2.Set(0, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
			vasynth.lfo_amp_ = (k1);
			vasynth.SetLFO();
			}
			if (abs(oldk2 - k2) > 0.0005f)
			{
			vasynth.lfo_freq_ = (k2);
			vasynth.SetLFO();
			}
			
            break;
        case 17:
		
//------------------LFO Waveform and target--------
			hardware.led1.Set(0, 0, 1);
			hardware.led2.Set(1, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				if (k1 < 0.125f)
				{
					vasynth.lfo_waveform_ = 0;
				}
					else if (k1 > 0.125f && k1 < 0.25f)
				{
					vasynth.lfo_waveform_ = 1;
				}
				else if (k1 > 0.25f && k1 < 0.375f)
				{
					vasynth.lfo_waveform_ = 2;
				}
				else if (k1 > 0.375f && k1 < 0.5f)
				{
					vasynth.lfo_waveform_ = 3;
				}
				else if (k1 > 0.5f && k1 < 0.625f)
				{
					vasynth.lfo_waveform_ = 4;
				}
				else if (k1 > 0.625f && k1 < 0.75f)
				{
					vasynth.lfo_waveform_ = 5;
				}
				else if (k1 > 0.75f && k1 < 0.875f)
				{
					vasynth.lfo_waveform_ = 6;
				}
				else if (k1 > 0.875f && k1 < 1.0f)
				{
					vasynth.lfo_waveform_ = 7;
				}
				vasynth.SetLFO();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				if (k2 < 0.25f)
				{
					vasynth.lfo_target_ = 0;
				}
					else if (k2 > 0.25f && k2 < 0.5f)
				{
					vasynth.lfo_target_ = 1;
				}
				else if (k2 > 0.5f && k2 < 0.75f)
				{
					vasynth.lfo_target_ = 2;
				}
				else if (k2 > 0.75f && k2 < 1.0f)
				{
					vasynth.lfo_target_ = 3;
				}
			}
            break;
        case 18:
			mode = 16;
            break;
		case 19:
			mode = 16;
			break;
		case 20:
			mode = 16;
			break;
		case 21:
			mode = 16;
			break;
		case 22:
			mode = 16;
			break;
		case 23:
			mode = 16;
			break;

		//Filter
		
//-------------------Frequency and res---------
		case 24:
			hardware.led1.Set(1, 1, 0);
			hardware.led2.Set(0, 0, 0);  
			if (abs(oldk1 - k1) > 0.0005f )
			{
			vasynth.filter_cutoff_ = cutoffParam.Process();
			vasynth.SetFilter();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
			vasynth.filter_res_ = (k2);
			vasynth.SetFilter();			
			}
            break;
			
//-----------------Filter Envelope Attack and Release----------			
        case 25:
			hardware.led1.Set(1, 1, 0);
			hardware.led2.Set(1, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
			vasynth.eg_f_attack_ = attackParam.Process();
			vasynth.SetEG();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
			vasynth.eg_f_release_ = releaseParam.Process();
			vasynth.SetEG();
			}

            break;
			
//----------------Filter Envelope Decay and Sustain----------
        case 26:
			hardware.led1.Set(1, 1, 0);
			hardware.led2.Set(0, 1, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
			vasynth.eg_f_decay_ = (k1) * 9;
			}
			if (abs(oldk2 - k2) > 0.0005f )
			vasynth.SetEG();				
			{
			vasynth.eg_f_sustain_ = (k2);
			}
            break;
			
//----------------Filter Type and Envelope Amount----------
		case 27:
			hardware.led1.Set(1, 1, 0);
			hardware.led2.Set(0, 0, 1);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				if (k1 < 0.2)
				{
					vasynth.filter_type_ = VASynth::LOW;
				}
				if (k1 > 0.2 && k1 < 0.4)
				{
					vasynth.filter_type_ = VASynth::HIGH;
				}
				if (k1 > 0.4 && k1 < 0.6)
				{
					vasynth.filter_type_ = VASynth::BAND;
				}
				if (k1 > 0.6 && k1 < 0.8)
				{
					vasynth.filter_type_ = VASynth::NOTCH;
				}
				if (k1 > 0.8)
				{
					vasynth.filter_type_ = VASynth::PEAK;
				}
			}
			
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.eg_f_amount_ = (k2)*100;
			}
			break;
		case 28:
			mode = 24;
			break;
		case 29:
			mode = 24;
			break;
		case 30:
			mode = 24;
			break;
		case 31:
			mode = 24;
			break;
			
			//FX
			
//----------Delay and Reverb Amount-------
		case 32:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(0, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.delay_level_ = k1;
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.reverb_level_ = k2;
			}
            break;
			
//-----------Delay Time and Feedback---------
		case 33:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(1, 0, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.delay_delay_ = k1 * 2.0f;
				vasynth.SetDelay();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.delay_feedback_ = k2;
				vasynth.SetDelay();

			}
            break;

//-------------Reverb Filter and Feedback---------
		case 34:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(0, 1, 0);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.reverb_lpffreq_  = cutoffParam.Process();
				vasynth.SetReverb();
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.reverb_feedback_ = k2;
				vasynth.SetReverb();
			}

            break;
			
//--------------Reverb Dry and Wet-------------
		case 35:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(0, 0, 1);
			if (abs(oldk1 - k1) > 0.0005f )
			{
				vasynth.reverb_dry_ = k1;
			}
			if (abs(oldk2 - k2) > 0.0005f )
			{
				vasynth.reverb_wet_ = k2;
			}

            break;
		case 36:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(1, 1, 0);
			mode = 32;

            break;
		case 37:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(0, 1, 1);

            break;
		case 38:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(1, 0, 1);
			if (abs(oldk1 - k1) > 0.0005f )
            break;
		case 39:
			hardware.led1.Set(0, 1, 1);
			hardware.led2.Set(1, 1, 1);

            break;
		case 40:
			mode = 0;
        default: break;
    }
}
