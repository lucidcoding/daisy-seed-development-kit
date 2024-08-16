#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "ParameterSet.h"
#include "../../Library/UiFramework/Presenters/Root.h"
#include "../../Library/UiFramework/Presenters/ListPage.h"
#include "../../Library/UiFramework/Presenters/NavigationListPageItem.h"
#include "../../Library/UiFramework/Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/NumericSettingsListPageItem.h"
#include "../../Library/UiFramework/Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/OptionsSettingsListPageItem.h"
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPage.h"
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/TabPage.h"
#include "../../Library/UiFramework/Presenters/TabPageItem.h"
#include "../../Library/UiFramework/Views/ListPageView.h"
#include "../../Library/UiFramework/Views/PotentiometerArrayPageView.h"
#include "../../Library/UiFramework/Views/TabPageView.h"
#include "../../Library/UiFramework/ViewAdapters/Ili9341ViewAdapter.h"
#include "../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::views;
using namespace developmentKit::library::uiFramework::viewAdapters;

/*

float sample_rate_;



   	voices_;

    --OSC/LFO
	waveform_;
	detune_;
	osc2_waveform_;
	osc2_transpose_;
	osc2_detune_;
	noise_level_;
	osc2_level_;
	portamento_;
	level_;
	pan_;
	lfo_waveform_;
	lfo_freq_;
	lfo_amp_;
	lfo_target_;


    --FILTER/ENV
	filter_type_;
	filter_cutoff_;
	filter_res_;
	eg_f_amount_;
	eg_f_attack_;
	eg_f_decay_;
	eg_f_sustain_; 
	eg_f_release_;
	eg_a_attack_;
	eg_a_decay_;
	eg_a_sustain_; 
	eg_a_release_;
	eg_p_attack_;
	eg_p_decay_;
	eg_p_sustain_; 
	eg_p_release_;


    --FX
	reverb_level_;
	reverb_dry_;
	reverb_wet_;
	reverb_feedback_;
	reverb_lpffreq_;
	delay_delay_;
	delay_feedback_;
	delay_level_;

*/
class UserInterface : public Root
{
public:
    UserInterface() {}
    ~UserInterface() {}
    void Init(UiDriver *prmUiDriver);
    ParameterSet GetParameters();
    void UpdateSettings(ParameterSet parameterSet);

private:
    UiDriver *uiDriver;
    ListPageView listPageView;
    TabPageView tabPageView;
    PotentiometerArrayPageView potentiometerArrayPageView;
    Ili9341ViewAdapter viewAdapter;
    TabPage tabPage;
    TabPageItem oscillatorTabPageItem;
    PotentiometerArrayPage oscillatorPotentiometerArrayPage;
    OptionsPotentiometerArrayPageItem osc1WaveShapePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem osc1DetunePotentiometerArrayPageItem;
    OptionsPotentiometerArrayPageItem osc2WaveShapePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem osc2DetunePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem portamentoPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem voicesPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem osc2TransposePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem osc2LevelPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem pitchAttackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem pitchDecayPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem pitchSustainPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem pitchReleasePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem noiseLevelPotentiometerArrayPageItem;

    TabPageItem filterAmpTabPageItem;
    PotentiometerArrayPage filterAmpPotentiometerArrayPage;
    NumericPotentiometerArrayPageItem filterAttackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterDecayPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterSustainPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterReleasePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem cutOffPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem resonancePotentiometerArrayPageItem;
    OptionsPotentiometerArrayPageItem filterTypePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterEnvelopeAmountPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem ampAttackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem ampDecayPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem ampSustainPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem ampReleasePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem panPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem mixLevelPotentiometerArrayPageItem;

    TabPageItem lfoFxTabPageItem;
    PotentiometerArrayPage lfoFxPotentiometerArrayPage;
    NumericPotentiometerArrayPageItem lfoLevelPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem lfoFrequencyPotentiometerArrayPageItem;
    OptionsPotentiometerArrayPageItem lfoWaveShapePotentiometerArrayPageItem;
    OptionsPotentiometerArrayPageItem lfoTargetPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem reverbAmountPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem reverbFeedbackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem reverbFilterPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem reverbDryPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem reverbWetPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem delayAmountPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem delayFeedbackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem delayTimePotentiometerArrayPageItem;


    TabPageItem quickTabPageItem;
    PotentiometerArrayPage quickPotentiometerArrayPage;
    ListPage settingsListPage;
    NumericSettingsListPageItem midiChannelSettingsListPageItem;
    OptionsSettingsListPageItem knobModeSettingsListPageItem;
};

#endif


