#pragma once
#ifndef DSY_VAVOICE_H
#define DSY_VAVOICE_H

#include "daisy_pod.h"
#include "daisysp.h"

#include "main.h"

// preset
#define PRESET_MAX 6
#define CATCH_THRESH 0.05f

using namespace daisy;
using namespace daisysp;


typedef struct
{
	// config
	
   	uint8_t voices;
	uint8_t waveform;
	float detune;
	
	uint8_t filter_type;
	float filter_res;
	float filter_cutoff;
	float eg_f_amount;
	
	float eg_a_attack;
	float eg_a_decay;
	float eg_a_sustain; // level
	float eg_a_release;
	float eg_f_attack;
	float eg_f_decay;
	float eg_f_sustain; // level
	float eg_f_release;
	float eg_p_attack;
	float eg_p_decay;
	float eg_p_sustain; // level
	float eg_p_release;
	
	
	uint8_t lfo_waveform;
	float lfo_freq;
	float lfo_amp;
	uint8_t lfo_target;
	
	float portamento;
	float noise_level;
	
	uint8_t osc2_waveform;
	float osc2_detune;
	float osc2_level;
	int8_t osc2_transpose;
	
	float pan;
	float level;
	
	float reverb_level;
	float reverb_dry;
	float reverb_wet;
	float reverb_feedback;
	float reverb_lpffreq;
	float delay_delay;
	float delay_feedback;
	float delay_level;
	
	uint8_t input_channel;
	uint8_t midi_channel;
	uint8_t pot0_target;
	uint8_t pot1_target;
} VASynthSetting;


class VASynth
{

	public:

    VASynth() {}
    ~VASynth() {}

    enum Waveform
    {
        WAVE_SIN,
        WAVE_TRI,
        WAVE_SAW,
        WAVE_RAMP,
        WAVE_SQUARE,
        WAVE_POLYBLEP_TRI,
        WAVE_POLYBLEP_SAW,
        WAVE_POLYBLEP_SQUARE,
        WAVE_LAST,
    };

    enum Target
    {
    	NONE,
        PITCH,
        FILTER,
        EG,
		PAN,
        LAST,
    };

	enum FilterType
	{
		BAND,
		HIGH,
		LOW,
		NOTCH,
		PEAK
	};

	void Init();
	void First();
	void SetWaveform();
	void SetEG();
	void SetFilter();
	void SetReverb();
	void SetDelay();
	void SetLFO();
	void SetLFOAmp(float lfo_amp_);
	void UpdateKnobs();
	void UpdateEncoder();
	void Controls();
	void UpdateButtons();
	void UpdateLeds();
	void Process(float *, float *);
	void NoteOn(uint8_t midi_note, uint8_t midi_velocity = MIDI_VELOCITY_MAX);
	void NoteOff(uint8_t midi_note);
	void FlashLoad(uint8_t aSlot);
	void FlashSave(uint8_t aSlot);
	void SaveToLive(VASynthSetting *);
	void LiveToSave(VASynthSetting *);
	uint8_t save_slot;

//private:

	// config

	float sample_rate_;
   	uint8_t voices_;
	uint8_t waveform_;
	float detune_;
	uint8_t filter_type_;
	float filter_res_;
	float filter_cutoff_;
	float eg_a_attack_;
	float eg_a_decay_;
	float eg_a_sustain_; // level
	float eg_a_release_;
	float eg_f_attack_;
	float eg_f_decay_;
	float eg_f_amount_;
	float eg_f_sustain_; // level
	float eg_f_release_;
	float eg_p_attack_;
	float eg_p_decay_;
	float eg_p_sustain_; // level
	float eg_p_release_;
	uint8_t lfo_waveform_;
	float lfo_freq_;
	float lfo_amp_;
	uint8_t lfo_target_;
	float portamento_;
	float noise_level_;
	uint8_t osc2_waveform_;
	float osc2_detune_;
	float osc2_level_;
	int8_t osc2_transpose_;

	float pan_;
	float level_;
	float reverb_level_;
	float reverb_dry_;
	float reverb_wet_;
	float reverb_feedback_;
	float reverb_lpffreq_;
	float delay_delay_;
	float delay_feedback_;
	float delay_level_;
	
	
	
	uint8_t input_channel_;
	uint8_t midi_channel_;
	uint8_t pot0_target_;
	uint8_t pot1_target_;
	
	// runtime	
	uint8_t osc_next_;
	uint8_t note_midi_[VOICES_MAX];
	float note_freq_[VOICES_MAX];

	
	Oscillator osc_[VOICES_MAX];
	Adsr eg_a_[VOICES_MAX];
	Adsr eg_f_[VOICES_MAX];
	Adsr eg_p_[VOICES_MAX];
	Svf svf_[VOICES_MAX];
	Oscillator lfo_;
	Port port_[VOICES_MAX];
	WhiteNoise noise_[VOICES_MAX];
	
	Oscillator osc2_[VOICES_MAX];

};

#endif
