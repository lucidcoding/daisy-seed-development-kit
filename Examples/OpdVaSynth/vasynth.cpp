#include "daisy_pod.h"
#include "daisy_seed.h"
#include "daisysp.h"

#include "main.h"
#include "vasynth.h"
//#include "qspi.h"


using namespace daisy;
using namespace daisysp;

// globals
extern DaisyPod hardware;
extern OpdModSources modSources;
extern float sysSampleRate;
extern uint8_t gPlay;
DaisySeed hw;

float lfoFreq, lfoAmp, oscWave;

static Parameter lfoFreqParam, lfoAmpParam, oscWaveParam;


// fx
ReverbSc reverb_;
static DelayLine<float, DELAY_MAX> DSY_SDRAM_BSS delay_;

// presets

uint8_t preset_max = PRESET_MAX;
uint8_t preset_number = 0;
char preset_name[PRESET_MAX][15] = {"Space pad", "Harp", "Port Solo", "Asgard", "Kajsa", "Lowlife"};
VASynthSetting preset_setting[PRESET_MAX] = {
{VOICES_OK, (uint8_t)VASynth::WAVE_SAW, 0.0f, VASynth::LOW, 0.3f, 3000.0f, 1.0f,
0, 0, 1.0f, 1.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0, 0, 0, 0,
(uint8_t)VASynth::WAVE_TRI, 0.1f, 0.4f, VASynth::FILTER, 0, 0.2f,
(uint8_t)VASynth::WAVE_TRI, 2.0f, 0.6f, 12, 0.5f, 0.3f,
0.5f, 0.2f, 0.8f, 0.8f, 6000, 0.8f, 0.3f, 1.0f,
INPUT_CHANNEL_NONE, MIDI_CHANNEL_ALL, POT_TARGET_NONE, POT_TARGET_NONE}
,
{VOICES_OK, (uint8_t)VASynth::WAVE_TRI, 0.0f, VASynth::LOW, 0.5f, 2000.0f, 1.0f,
0.0f, 0.3f, 0.0f, 0.3f,   0.0f, 0.3f, 0.0f, 0.3f,   0.3f, 0.3f, 0.0f, 0.3f,
(uint8_t)VASynth::WAVE_TRI, 0.0f, 0.0f, VASynth::FILTER, 0, 0.0f,
(uint8_t)VASynth::WAVE_TRI, 5.0f, 0.3f, 0, 0.5f, 0.5f,
0.8f, 0.8f, 0.2f, 0.4f, 6000, 1.0f, 0.5f, 1.0f,
INPUT_CHANNEL_NONE, MIDI_CHANNEL_ALL, POT_TARGET_NONE, POT_TARGET_NONE}
,
{1, (uint8_t)VASynth::WAVE_TRI, 0.0f, VASynth::LOW, 0.5f, 2000.0f, 1.0f,
0.5f, 0.3f, 1.0f, 0.8f,   0.3f, 0.0f, 0.3f, 0.3f,  0, 0, 0, 0,
(uint8_t)VASynth::WAVE_TRI, 0.1f, 0.5f, VASynth::FILTER, 0.05f, 0.1f,
(uint8_t)VASynth::WAVE_TRI, 5.0f, 0.3f, 12, 0.5f, 0.1f,
0.8f, 0.5f, 0.5f, 0.4f, 4000, 1.0f, 0.5f, 1.0f,
INPUT_CHANNEL_NONE, MIDI_CHANNEL_ALL, POT_TARGET_NONE, POT_TARGET_NONE}
,
{VOICES_OK, (uint8_t)VASynth::WAVE_RAMP, 0.0f, VASynth::HIGH, 0.6f, 1000.0f, 1.0f,
0, 0, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 2.0f,   0, 0, 0, 0,
(uint8_t)VASynth::WAVE_TRI, 2.0f, 0.20f, VASynth::FILTER, 0.0f, 0.1f,
(uint8_t)VASynth::WAVE_SIN, 3.0f, 0.6f, 0, 0.5f, 0.3f,
0.5f, 0.2f, 0.8f, 0.6f, 3000, 0.05f, 0.2f, 1.0f,
INPUT_CHANNEL_NONE, MIDI_CHANNEL_ALL, POT_TARGET_NONE, POT_TARGET_NONE}
,
{VOICES_OK, VASynth::WAVE_SAW, 0.0f, VASynth::LOW, 0.2f, 1000.0f, 1.0f,
0.0f, 0.1f, 1.0f, 0.8f,   0.2f, 0.01f, 1.0f, 0.8f,  0.5f, 1.0f, 0.0f, 1.0f,
(uint8_t)VASynth::WAVE_TRI, 0.5f, 0.4f, VASynth::FILTER, 0.0f, 0.3f,
(uint8_t)VASynth::WAVE_SAW, 2.0f, 0.5f, 12, 0.5f, 0.3f,
0.2f, 0.20, 0.80, 0.8f, 6000, 0.8f, 0.4f, 1.0f,
INPUT_CHANNEL_NONE, MIDI_CHANNEL_ALL, POT_TARGET_NONE, POT_TARGET_NONE}
,
{VOICES_OK, VASynth::WAVE_RAMP, 0.0f, VASynth::LOW, 0.2f, 2000.0f, 1.0f,
0.0f, 0.0f, 1.0f, 0.4f,   0.0f, 0.0f, 1.0f, 0.4f,  0.0f, 0.0f, 0.0f, 0.0f,
(uint8_t)VASynth::WAVE_TRI, 0.5f, 0.5f, VASynth::NONE, 0.0f, 0.0f,
(uint8_t)VASynth::WAVE_SIN, 0.0f, 0.7f, 0, 0.5f, 0.4f,
0.0f, 0.80, 0.20, 0.0f, 1000, 0.05f, 0.2f, 1.0f,
INPUT_CHANNEL_NONE, MIDI_CHANNEL_ALL, POT_TARGET_NONE, POT_TARGET_NONE}
};

/*
template:

voices, waveform, detune, filter_type, filter_res, filter_cutoff, filter envelope amount,
eg_a_attack, d, s, r, eg_f_attack, d, s, r, eg_p_attack, d, s, r, 
lfo_waveform, lfo_freq, lfo_amp, lfo_target, portamento, noise_level,
osc2_waveform, osc2_detune, osc2_level, osc2_transpose, pan, level,
reverb_level, reverb_dry, reverb_wet, reverb_feedback, reverb_lpffreq, delay_delay, delay_feedback, delay_level, 
input_channel, 	midi_channel, pot0_target, pot1_target;
*/


void VASynth::Init()
{
save_slot = 0;
	for (uint8_t i = 0; i < voices_; i++)
	{

		
		// oscillator
		
		osc_[i].Init(sample_rate_);
		osc_[i].SetWaveform(waveform_);
		osc_[i].SetAmp(0.5f); // default
		osc_[i].SetFreq(440.0f); // default

		osc2_[i].Init(sample_rate_);
		osc2_[i].SetWaveform(osc2_waveform_);
		osc2_[i].SetAmp(osc2_level_); // default
		osc2_[i].SetFreq(440.0f * osc2_detune_); // default
		
		// EG - pitch
		
		eg_p_[i].Init(sample_rate_);
		eg_p_[i].SetTime(ADSR_SEG_ATTACK, eg_p_attack_);
		eg_p_[i].SetTime(ADSR_SEG_DECAY, eg_p_decay_);
		eg_p_[i].SetTime(ADSR_SEG_RELEASE, eg_p_release_);
		eg_p_[i].SetSustainLevel(eg_p_sustain_);

		// EG - amplitude
		
		eg_a_[i].Init(sample_rate_);
		eg_a_[i].SetTime(ADSR_SEG_ATTACK, eg_a_attack_);
		eg_a_[i].SetTime(ADSR_SEG_DECAY, eg_a_decay_);
		eg_a_[i].SetTime(ADSR_SEG_RELEASE, eg_a_release_);
		eg_a_[i].SetSustainLevel(eg_a_sustain_);

		// EG - filter
		
		eg_f_[i].Init(sample_rate_);
		eg_f_[i].SetTime(ADSR_SEG_ATTACK, eg_f_attack_);
		eg_f_[i].SetTime(ADSR_SEG_DECAY, eg_f_decay_);
		eg_f_[i].SetTime(ADSR_SEG_RELEASE, eg_f_release_);
		eg_f_[i].SetSustainLevel(eg_f_sustain_);

		// filter

		svf_[i].Init(sample_rate_);
		svf_[i].SetFreq(filter_cutoff_);
		svf_[i].SetRes(filter_res_);
		svf_[i].SetDrive(0.0f); // default
	
		// portamento
		
		port_[i].Init(sample_rate_, portamento_);
		
		// noise
		
		noise_[i].Init();
	}

	// lfo

	lfo_.Init(sample_rate_);
	lfo_.SetWaveform(lfo_waveform_);
	lfo_.SetFreq(lfo_freq_);
	lfo_.SetAmp(lfo_amp_);
	
	//
	reverb_.Init(sample_rate_);
	reverb_.SetFeedback(reverb_feedback_);
	reverb_.SetLpFreq(reverb_lpffreq_);
	delay_.Init();
	delay_.SetDelay(sample_rate_ * delay_delay_);

    // init
    osc_next_ = 0; // circular buffer of midi notes
}



void VASynth::First()
{
	sample_rate_ = sysSampleRate;
	SaveToLive(&preset_setting[5]);

}	


void VASynth::SetWaveform()
{
	
	for (uint8_t i = 0; i < voices_; i++)
	{
		osc_[i].SetWaveform(waveform_);
		osc2_[i].SetWaveform(osc2_waveform_);
		port_[i].SetHtime(portamento_);
	}

}







void VASynth::SetEG()
{
	for (uint8_t i = 0; i < voices_; i++)
	{
		// EG - pitch
		
		eg_p_[i].SetTime(ADSR_SEG_ATTACK, eg_p_attack_);
		eg_p_[i].SetTime(ADSR_SEG_DECAY, eg_p_decay_);
		eg_p_[i].SetTime(ADSR_SEG_RELEASE, eg_p_release_);
		eg_p_[i].SetSustainLevel(eg_p_sustain_);

		// EG - filter
		
		eg_f_[i].SetTime(ADSR_SEG_ATTACK, eg_f_attack_);
		eg_f_[i].SetTime(ADSR_SEG_DECAY, eg_f_decay_);
		eg_f_[i].SetTime(ADSR_SEG_RELEASE, eg_f_release_);
		eg_f_[i].SetSustainLevel(eg_f_sustain_);

		// EG - amplitude
		
		eg_a_[i].SetTime(ADSR_SEG_ATTACK, eg_a_attack_);
		eg_a_[i].SetTime(ADSR_SEG_DECAY, eg_a_decay_);
		eg_a_[i].SetTime(ADSR_SEG_RELEASE, eg_a_release_);
		eg_a_[i].SetSustainLevel(eg_a_sustain_);
	}
}



void VASynth::SetFilter()
{
	for (uint8_t i = 0; i < voices_; i++)
	{
		svf_[i].SetFreq(filter_cutoff_);
		svf_[i].SetRes(filter_res_);
	}

}



void VASynth::SetDelay()
{
	delay_.SetDelay(sample_rate_ * delay_delay_);
}



void VASynth::SetReverb()
{
	reverb_.SetFeedback(reverb_feedback_);
	reverb_.SetLpFreq(reverb_lpffreq_);
}



void VASynth::SetLFO()
{
	lfo_.SetWaveform(lfo_waveform_);
	lfo_.SetFreq(lfo_freq_*10);
	lfo_.SetAmp(lfo_amp_);
	
}

void VASynth::SetLFOAmp(float lfo_amp_)
{

	
}



/*
static float LOG2 = log(2);

int freqToPitch(float freq)
{
  return int(69+12*log(freq/440)/LOG2);
} // freqToPitch()

float pitch2Freq (int p)
{
  return pow(2,(float)(p-69)/12) * 440;
} // pitch2Freq()
*/


void VASynth::Process(float *out_l, float *out_r)
{
	
	float lfo_out;
	float env_a_out;
	float env_f_out;
	float osc_out;
	float filter_out;
	float voice_out;
	float voice_left, voice_right;
	float delay_out;
	float rev_out_left, rev_out_right;

	float porta_freq;
	bool note_on;

	// lfo + apply
	lfo_out = lfo_.Process();
	
	filter_out = 0;
	for (uint8_t i = 0; i < voices_; i++)
	{
		note_on = (note_midi_[i] != 0);

		// EG - AMP
		if (lfo_target_ == EG)
		{
			env_a_out = eg_a_[i].Process(note_on) + lfo_out;
		} else {
			env_a_out = eg_a_[i].Process(note_on);
		}
		osc_[i].SetAmp(env_a_out);
		osc2_[i].SetAmp(env_a_out);
	
		// pitch
	
		// osc - portamento
		if (portamento_ > 0.0f)
		{
			porta_freq = port_[i].Process(note_freq_[i]);
			osc_[i].SetFreq(porta_freq + detune_);
			//osc2_[i].SetFreq(pitch2Freq(freqToPitch(porta_freq) + osc2_transpose_) + osc2_detune_);
		} else if (lfo_target_ == PITCH)
		{
			// lfo - pitch
			osc_[i].SetFreq((note_freq_[i] + detune_) + lfo_out * 10);
			osc2_[i].SetFreq((note_freq_[i] + osc2_detune_) + lfo_out * 10);
		} else if (eg_p_sustain_ > 0.0f)
		{
			// EG - pitch
			osc_[i].SetFreq((note_freq_[i] + detune_) + eg_p_[i].Process(note_on));
			osc2_[i].SetFreq((note_freq_[i] + osc2_detune_) + eg_p_[i].Process(note_on));
		}

		// osc + osc2 + noise
		osc_out = osc_[i].Process() + (osc2_[i].Process() * osc2_level_) + (noise_[i].Process() * noise_level_ * env_a_out);

		// filter
		
		env_f_out = eg_f_[i].Process(note_on);

		if (lfo_target_ == FILTER) {
			svf_[i].SetFreq(filter_cutoff_ * (1 - lfo_out));
		} else {
			svf_[i].SetFreq(filter_cutoff_ * (env_f_out * (eg_f_amount_+ 1)));
		}

		svf_[i].Process(osc_out);
		switch (filter_type_)
		{
		case BAND:
			filter_out += svf_[i].Band();
			break;
		case HIGH:
			filter_out += svf_[i].High();
			break;
		case LOW:
			filter_out += svf_[i].Low();
			break;
		case NOTCH:
			filter_out += svf_[i].Notch();
			break;
		case PEAK:
			filter_out += svf_[i].Peak();
			break;
		default:
			filter_out += osc_out;
		}
	}
	filter_out /= voices_;

	voice_out = filter_out * level_;

	// delay
	if (delay_feedback_ > 0.0f)
	{
		delay_out = delay_.Read();
		delay_.Write((voice_out + delay_out) * delay_feedback_);
		voice_out += delay_out * delay_level_;
	}

	// pan
	if (lfo_target_ == PAN) {

		voice_left = voice_out * ((1.0f - pan_) * lfo_out);
		voice_right = voice_out * (pan_ * lfo_out);
		
	} else {
		
		voice_left = voice_out * (1.0f - pan_);
		voice_right = voice_out * (pan_);
	}


	// reverb
	reverb_.Process(voice_left * reverb_level_, voice_right * reverb_level_, &rev_out_left, &rev_out_right);

	// out

	*out_l = voice_left * reverb_dry_ + rev_out_left * reverb_wet_;
	*out_r = voice_right * reverb_dry_ + rev_out_right * reverb_wet_;

}



void VASynth::NoteOn(uint8_t midi_note, uint8_t midi_velocity)
{
	osc_next_ = (osc_next_ + 1) % voices_;

	note_midi_[osc_next_] = midi_note;
	note_freq_[osc_next_] = mtof(note_midi_[osc_next_]);
	osc_[osc_next_].SetFreq(note_freq_[osc_next_]);

	int8_t midi2 = note_midi_[osc_next_] + osc2_transpose_;
	if (midi2 < 0 || midi2 > 127)
	{
		midi2 = midi_note;
	}
	osc2_[osc_next_].SetFreq(mtof(midi2) * osc2_detune_);

	eg_a_[osc_next_].SetSustainLevel(((float)midi_velocity / MIDI_VELOCITY_MAX) * eg_a_sustain_);
}	



void VASynth::NoteOff(uint8_t midi_note)
{
	for (uint8_t i = 0; i < voices_; i++)
	{
		if (note_midi_[i] == midi_note)
		{
			note_midi_[i] = 0;
		}
	}
}	


// Flash handling - load and save
// 8MB of flash
// 4kB blocks
// assume our settings < 4kB, so put one patch per block
#define FLASH_BLOCK 4096

uint8_t DSY_QSPI_BSS qspi_buffer[FLASH_BLOCK * 16];
 
void VASynth::FlashLoad(uint8_t aSlot)
{
	VASynthSetting vaload;

    size_t size = sizeof(VASynthSetting);
    
	//memcpy(*dest, *src, sizet);
	memcpy(&vaload, &qspi_buffer[aSlot * FLASH_BLOCK], size);

	SaveToLive(&vaload);
}



void VASynth::FlashSave(uint8_t aSlot)
{
	VASynthSetting vasave;

	LiveToSave(&vasave);

	size_t start_address = (size_t)qspi_buffer;

	size_t size = sizeof(VASynthSetting);
    
	size_t slot_address = start_address + (aSlot * FLASH_BLOCK);

    hw.qspi.Erase(slot_address, slot_address + size);
    hw.qspi.Write(slot_address, size, (uint8_t*)&vasave);

}



void VASynth::SaveToLive(VASynthSetting *vas)
{
	gPlay = PLAY_OFF;
	
	//sample_rate_ = vas->sample_rate;
	voices_ = vas->voices;
	waveform_ = vas->waveform;
	detune_ = vas->detune;
	// more
	filter_type_ = vas->filter_type;
	filter_res_ = vas->filter_res;
	filter_cutoff_ = vas->filter_cutoff;
	eg_f_amount_ = vas->eg_f_amount;
	
	eg_a_attack_ = vas->eg_a_attack;
	eg_a_decay_ = vas->eg_a_decay;
	eg_a_sustain_ = vas->eg_a_sustain; // level
	eg_a_release_ = vas->eg_a_release;
	eg_f_attack_ = vas->eg_f_attack;
	eg_f_decay_ = vas->eg_f_decay;
	eg_f_sustain_ = vas->eg_f_sustain; // level
	eg_f_release_ = vas->eg_f_release;
	eg_p_attack_ = vas->eg_p_attack;
	eg_p_decay_ = vas->eg_p_decay;
	eg_p_sustain_ = vas->eg_p_sustain; // level
	eg_p_release_ = vas->eg_p_release;
	
	lfo_waveform_ = vas->lfo_waveform;
	lfo_freq_ = vas->lfo_freq;
	lfo_amp_ = vas->lfo_amp;
	lfo_target_ = vas->lfo_target;

	portamento_ = vas->portamento;
	noise_level_ = vas->noise_level;

	osc2_waveform_ = vas->osc2_waveform;
	osc2_detune_ = vas->osc2_detune;
	osc2_level_ = vas->osc2_level;
	osc2_transpose_ = vas->osc2_transpose;
	
	pan_ = vas->pan;
	level_ = vas->level;
	
	reverb_level_ = vas->reverb_level;
	reverb_dry_ = vas->reverb_dry;
	reverb_wet_ = vas->reverb_wet;
	reverb_feedback_ = vas->reverb_feedback;
	reverb_lpffreq_ = vas->reverb_lpffreq;
	delay_delay_ = vas->delay_delay;
	delay_feedback_ = vas->delay_feedback;
	delay_level_ = vas->delay_level;
	
	input_channel_ = vas->input_channel;
	midi_channel_ = vas->midi_channel;
	pot0_target_ = vas->pot0_target;
	pot1_target_ = vas->pot1_target;

	osc_next_ = 0; // circular buffer of midi notes
	Init();
	
	gPlay = PLAY_ON;
}

void VASynth::LiveToSave(VASynthSetting *vas)
{
	//vas->sample_rate = sample_rate_;
	vas->voices = voices_;
	vas->waveform = waveform_;
	vas->detune = detune_;

	vas->filter_type = filter_type_;
	vas->filter_res = filter_res_;
	vas->filter_cutoff = filter_cutoff_;
	vas->eg_f_amount = eg_f_amount_;
	
	vas->eg_a_attack = eg_a_attack_;
	vas->eg_a_decay = eg_a_decay_;
	vas->eg_a_sustain = eg_a_sustain_;
	vas->eg_a_release = eg_a_release_;
	vas->eg_f_attack = eg_f_attack_;
	vas->eg_f_decay = eg_f_decay_;
	vas->eg_f_sustain = eg_f_sustain_;
	vas->eg_f_release = eg_f_release_;
	vas->eg_p_attack = eg_p_attack_;
	vas->eg_p_decay = eg_p_decay_;
	vas->eg_p_sustain = eg_p_sustain_;
	vas->eg_p_release = eg_p_release_;
	
	vas->lfo_waveform = lfo_waveform_;
	vas->lfo_freq = lfo_freq_;
	vas->lfo_amp = lfo_amp_;
	vas->lfo_target = lfo_target_;

	vas->portamento = portamento_;
	vas->noise_level = noise_level_;

	vas->osc2_waveform = osc2_waveform_;
	vas->osc2_detune = osc2_detune_;
	vas->osc2_level = osc2_level_;
	vas->osc2_transpose = osc2_transpose_;

	vas->pan = pan_;
	vas->level = level_;
	
	vas->reverb_level = reverb_level_;
	vas->reverb_dry = reverb_dry_;
	vas->reverb_wet = reverb_wet_;
	vas->reverb_feedback = reverb_feedback_;
	vas->reverb_lpffreq = reverb_lpffreq_;
	vas->delay_delay = delay_delay_;
	vas->delay_feedback = delay_feedback_;
	vas->delay_level = delay_level_;
	
	vas->input_channel = input_channel_;
	vas->midi_channel = midi_channel_;
	vas->pot0_target = pot0_target_;
	vas->pot1_target = pot0_target_;
}



