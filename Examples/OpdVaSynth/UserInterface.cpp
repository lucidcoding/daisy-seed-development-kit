#include "UserInterface.h"
#include "ParameterSet.h"
#include "../../Library/UiFramework/Presenters/NavigationListPageItem.h"
#include "../../Library/UiFramework/Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPage.h"
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/TabPage.h"
#include "../../Library/UiFramework/Presenters/TabPageItem.h"
#include "../../Library/UiFramework/Views/ListPageView.h"
#include "../../Library/UiFramework/Views/PotentiometerArrayPageView.h"
#include "../../Library/UiFramework/Views/TabPageView.h"
#include "../../Library/UiFramework/ViewAdapters/Ili9341ViewAdapter.h"
#include "../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"
#include "vasynth.h"

using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::views;
using namespace developmentKit::library::uiFramework::viewAdapters;

void UserInterface::Init(UiDriver *prmUiDriver)
{
    // Initialise hardware
    uiDriver = prmUiDriver;
    uiDriver->Init();

    // Initiviews
    viewAdapter.Init(uiDriver);
    tabPageView.Init(&viewAdapter, 0, 0, 320, 240);
    listPageView.Init(&viewAdapter, 21, 21, 278, 198);
    potentiometerArrayPageView.Init(&viewAdapter, 12, 22, 296, 210);
    tabPage.Init(&tabPageView);
    oscillatorPotentiometerArrayPage.Init(&potentiometerArrayPageView);
    filterAmpPotentiometerArrayPage.Init(&potentiometerArrayPageView);
    lfoPotentiometerArrayPage.Init(&potentiometerArrayPageView);
    quickPotentiometerArrayPage.Init(&potentiometerArrayPageView);
    settingsListPage.Init(&listPageView);

    // Add oscillator tab to tab page
    oscillatorTabPageItem.Init("OSC", &oscillatorPotentiometerArrayPage);
    tabPage.AddItem(&oscillatorTabPageItem);

    // Add oscillator 1 wave shape to oscillator tab
    osc1WaveShapePotentiometerArrayPageItem.AddOption("SIN", VASynth::WAVE_SIN);
    osc1WaveShapePotentiometerArrayPageItem.AddOption("TRI", VASynth::WAVE_TRI);
    osc1WaveShapePotentiometerArrayPageItem.AddOption("SAW", VASynth::WAVE_SAW);
    osc1WaveShapePotentiometerArrayPageItem.AddOption("SQU", VASynth::WAVE_SQUARE);
    osc1WaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc1WaveShapePotentiometerArrayPageItem.SetTitle("OSC 1 WAVE");
    oscillatorPotentiometerArrayPage.SetItem(0, &osc1WaveShapePotentiometerArrayPageItem);

    // Add oscillator 1 detune to oscillator tab
    osc1DetunePotentiometerArrayPageItem.SetScaledRange(0, 255);
    osc1DetunePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc1DetunePotentiometerArrayPageItem.SetTitle("OSC 1 DETUNE");
    oscillatorPotentiometerArrayPage.SetItem(1, &osc1DetunePotentiometerArrayPageItem);

    // Add oscillator 2 wave shape to oscillator tab
    osc2WaveShapePotentiometerArrayPageItem.AddOption("SIN", VASynth::WAVE_SIN);
    osc2WaveShapePotentiometerArrayPageItem.AddOption("TRI", VASynth::WAVE_TRI);
    osc2WaveShapePotentiometerArrayPageItem.AddOption("SAW", VASynth::WAVE_SAW);
    osc2WaveShapePotentiometerArrayPageItem.AddOption("SQU", VASynth::WAVE_SQUARE);
    osc2WaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2WaveShapePotentiometerArrayPageItem.SetTitle("OSC 2 WAVE");
    oscillatorPotentiometerArrayPage.SetItem(2, &osc2WaveShapePotentiometerArrayPageItem);

    // Add oscillator 2 fine tune to oscillator tab
    osc2DetunePotentiometerArrayPageItem.SetScaledRange(0, 255);
    osc2DetunePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2DetunePotentiometerArrayPageItem.SetTitle("OSC 2 FINE");
    oscillatorPotentiometerArrayPage.SetItem(3, &osc2DetunePotentiometerArrayPageItem);

    // Add portamento to oscillator tab
    portamentoPotentiometerArrayPageItem.SetScaledRange(0, 255);
    portamentoPotentiometerArrayPageItem.SetOutputValue(0.5f);
    portamentoPotentiometerArrayPageItem.SetTitle("PORTAMENTO");
    oscillatorPotentiometerArrayPage.SetItem(4, &portamentoPotentiometerArrayPageItem);

    // Add number of voices to oscillator tab
    voicesPotentiometerArrayPageItem.SetScaledRange(1, 8);
    voicesPotentiometerArrayPageItem.SetOutputValue(0.5f);
    voicesPotentiometerArrayPageItem.SetTitle("VOICES");
    oscillatorPotentiometerArrayPage.SetItem(5, &voicesPotentiometerArrayPageItem);

    // Add oscillator 2 coarse tune to oscillator tab
    osc2TransposePotentiometerArrayPageItem.SetScaledRange(-12, 12);
    osc2TransposePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2TransposePotentiometerArrayPageItem.SetTitle("OSC 2 TRANS");
    oscillatorPotentiometerArrayPage.SetItem(6, &osc2TransposePotentiometerArrayPageItem);

    // Add oscillator 2 coarse tune to oscillator tab
    osc2LevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    osc2LevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2LevelPotentiometerArrayPageItem.SetTitle("OSC 2 LEVEL");
    oscillatorPotentiometerArrayPage.SetItem(7, &osc2LevelPotentiometerArrayPageItem);

    // Add pitch attack to envelope tab
    pitchAttackPotentiometerArrayPageItem.SetScaledRange(0, 255);
    pitchAttackPotentiometerArrayPageItem.SetOutputValue(0.5f);
    pitchAttackPotentiometerArrayPageItem.SetTitle("PITCH ATTACK");
    oscillatorPotentiometerArrayPage.SetItem(8, &pitchAttackPotentiometerArrayPageItem);

    // Add pitch decay to envelope tab
    pitchDecayPotentiometerArrayPageItem.SetScaledRange(0, 255);
    pitchDecayPotentiometerArrayPageItem.SetOutputValue(0.5f);
    pitchDecayPotentiometerArrayPageItem.SetTitle("PITCH DECAY");
    oscillatorPotentiometerArrayPage.SetItem(9, &pitchDecayPotentiometerArrayPageItem);

    // Add pitch sustain to envelope tab
    pitchSustainPotentiometerArrayPageItem.SetScaledRange(0, 255);
    pitchSustainPotentiometerArrayPageItem.SetOutputValue(0.5f);
    pitchSustainPotentiometerArrayPageItem.SetTitle("PITCH SUSTAIN");
    oscillatorPotentiometerArrayPage.SetItem(10, &pitchSustainPotentiometerArrayPageItem);

    // Add pitch release to envelope tab
    pitchReleasePotentiometerArrayPageItem.SetScaledRange(0, 255);
    pitchReleasePotentiometerArrayPageItem.SetOutputValue(0.5f);
    pitchReleasePotentiometerArrayPageItem.SetTitle("PITCH RELEASE");
    oscillatorPotentiometerArrayPage.SetItem(11, &pitchReleasePotentiometerArrayPageItem);

    // Add noise level to oscillator tab
    noiseLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    noiseLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    noiseLevelPotentiometerArrayPageItem.SetTitle("NOISE LEVEL");
    noiseLevelPotentiometerArrayPageItem.SetQuickAlias("OSC:PW");
    oscillatorPotentiometerArrayPage.SetItem(12, &noiseLevelPotentiometerArrayPageItem);

    // Add envelope tab to tab page
    filterAmpTabPageItem.Init("ENV", &filterAmpPotentiometerArrayPage);
    tabPage.AddItem(&filterAmpTabPageItem);

    // Add attack to envelope tab
    filterAttackPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterAttackPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterAttackPotentiometerArrayPageItem.SetTitle("FILTER ATTACK");
    filterAmpPotentiometerArrayPage.SetItem(0, &filterAttackPotentiometerArrayPageItem);

    // Add decay to envelope tab
    filterDecayPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterDecayPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterDecayPotentiometerArrayPageItem.SetTitle("FILTER DECAY");
    filterAmpPotentiometerArrayPage.SetItem(1, &filterDecayPotentiometerArrayPageItem);

    // Add sustain to envelope tab
    filterSustainPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterSustainPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterSustainPotentiometerArrayPageItem.SetTitle("FILTER SUSTAIN");
    filterAmpPotentiometerArrayPage.SetItem(2, &filterSustainPotentiometerArrayPageItem);

    // Add release to envelope tab
    filterReleasePotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterReleasePotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterReleasePotentiometerArrayPageItem.SetTitle("FILTER RELEASE");
    filterAmpPotentiometerArrayPage.SetItem(3, &filterReleasePotentiometerArrayPageItem);

    // Add cut off to oscillator tab
    cutOffPotentiometerArrayPageItem.SetScaledRange(0, 255);
    cutOffPotentiometerArrayPageItem.SetOutputValue(0.5f);
    cutOffPotentiometerArrayPageItem.SetTitle("CUT OFF");
    filterAmpPotentiometerArrayPage.SetItem(4, &cutOffPotentiometerArrayPageItem);

    // Add resonance to oscillator tab
    resonancePotentiometerArrayPageItem.SetScaledRange(0, 255);
    resonancePotentiometerArrayPageItem.SetOutputValue(0.5f);
    resonancePotentiometerArrayPageItem.SetTitle("RESONANCE");
    filterAmpPotentiometerArrayPage.SetItem(5, &resonancePotentiometerArrayPageItem);

    // Add filter type to filter/amp tab
    filterTypePotentiometerArrayPageItem.AddOption("LPF", VASynth::LOW);
    filterTypePotentiometerArrayPageItem.AddOption("HPF", VASynth::HIGH);
    filterTypePotentiometerArrayPageItem.AddOption("BPF", VASynth::BAND);
    filterTypePotentiometerArrayPageItem.AddOption("NOTCH", VASynth::NOTCH);
    filterTypePotentiometerArrayPageItem.AddOption("PEAAK", VASynth::PEAK);
    filterTypePotentiometerArrayPageItem.SetTitle("FILTER TYPE");
    filterAmpPotentiometerArrayPage.SetItem(6, &filterTypePotentiometerArrayPageItem);

    // Add filter enveloper anount to filter/amp tab
    filterEnvelopeAmountPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterEnvelopeAmountPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterEnvelopeAmountPotentiometerArrayPageItem.SetTitle("FILT ENV AMT");
    filterAmpPotentiometerArrayPage.SetItem(7, &filterEnvelopeAmountPotentiometerArrayPageItem);

    // Add amp attack to envelope tab
    ampAttackPotentiometerArrayPageItem.SetScaledRange(0, 255);
    ampAttackPotentiometerArrayPageItem.SetOutputValue(0.5f);
    ampAttackPotentiometerArrayPageItem.SetTitle("AMP ATTACK");
    filterAmpPotentiometerArrayPage.SetItem(8, &ampAttackPotentiometerArrayPageItem);

    // Add decay to envelope tab
    ampDecayPotentiometerArrayPageItem.SetScaledRange(0, 255);
    ampDecayPotentiometerArrayPageItem.SetOutputValue(0.5f);
    ampDecayPotentiometerArrayPageItem.SetTitle("AMP DECAY");
    filterAmpPotentiometerArrayPage.SetItem(9, &ampDecayPotentiometerArrayPageItem);

    // Add sustain to envelope tab
    ampSustainPotentiometerArrayPageItem.SetScaledRange(0, 255);
    ampSustainPotentiometerArrayPageItem.SetOutputValue(0.5f);
    ampSustainPotentiometerArrayPageItem.SetTitle("AMP SUSTAIN");
    filterAmpPotentiometerArrayPage.SetItem(10, &ampSustainPotentiometerArrayPageItem);

    // Add release to envelope tab
    ampReleasePotentiometerArrayPageItem.SetScaledRange(0, 255);
    ampReleasePotentiometerArrayPageItem.SetOutputValue(0.5f);
    ampReleasePotentiometerArrayPageItem.SetTitle("AMP RELEASE");
    filterAmpPotentiometerArrayPage.SetItem(11, &ampReleasePotentiometerArrayPageItem);

    // Add pan to filter/amp tab
    panPotentiometerArrayPageItem.SetScaledRange(0, 255);
    panPotentiometerArrayPageItem.SetOutputValue(0.5f);
    panPotentiometerArrayPageItem.SetTitle("PAN");
    filterAmpPotentiometerArrayPage.SetItem(12, &panPotentiometerArrayPageItem);

    // Add mix level to filter/amp tab
    mixLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    mixLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    mixLevelPotentiometerArrayPageItem.SetTitle("LEVEL");
    filterAmpPotentiometerArrayPage.SetItem(13, &mixLevelPotentiometerArrayPageItem);

    // Add LFO tab to tab page
    lfoTabPageItem.Init("LFO", &lfoPotentiometerArrayPage);
    tabPage.AddItem(&lfoTabPageItem);

    // Add LFO level to LFO tab
    lfoLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    lfoLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    lfoLevelPotentiometerArrayPageItem.SetTitle("LEVEL");
    lfoPotentiometerArrayPage.SetItem(0, &lfoLevelPotentiometerArrayPageItem);

    // Add LFO frequency to LFO tab
    lfoFrequencyPotentiometerArrayPageItem.SetScaledRange(0, 255);
    lfoFrequencyPotentiometerArrayPageItem.SetOutputValue(0.5f);
    lfoFrequencyPotentiometerArrayPageItem.SetTitle("LEVEL");
    lfoPotentiometerArrayPage.SetItem(1, &lfoFrequencyPotentiometerArrayPageItem);

    // Add LFO wave shape to LFO tab
    lfoWaveShapePotentiometerArrayPageItem.AddOption("SIN", VASynth::WAVE_SIN);
    lfoWaveShapePotentiometerArrayPageItem.AddOption("TRI", VASynth::WAVE_TRI);
    lfoWaveShapePotentiometerArrayPageItem.AddOption("SAW", VASynth::WAVE_SAW);
    lfoWaveShapePotentiometerArrayPageItem.AddOption("SQU", VASynth::WAVE_RAMP);
    lfoWaveShapePotentiometerArrayPageItem.AddOption("SQU", VASynth::WAVE_SQUARE);
    lfoWaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    lfoWaveShapePotentiometerArrayPageItem.SetTitle("LFO WAVE");
    lfoPotentiometerArrayPage.SetItem(2, &lfoWaveShapePotentiometerArrayPageItem);

    // Add LFO target to LFO tab
    lfoTargetPotentiometerArrayPageItem.AddOption("NONE", VASynth::Target::NONE);
    lfoTargetPotentiometerArrayPageItem.AddOption("PITCH", VASynth::Target::PITCH);
    lfoTargetPotentiometerArrayPageItem.AddOption("FILTER", VASynth::Target::FILTER);
    lfoTargetPotentiometerArrayPageItem.AddOption("EG", VASynth::Target::EG);
    lfoTargetPotentiometerArrayPageItem.AddOption("PAN", VASynth::Target::PAN);
    lfoTargetPotentiometerArrayPageItem.AddOption("LAST", VASynth::Target::LAST);
    lfoTargetPotentiometerArrayPageItem.SetOutputValue(0.5f);
    lfoTargetPotentiometerArrayPageItem.SetTitle("LFO TARGET");
    lfoPotentiometerArrayPage.SetItem(3, &lfoTargetPotentiometerArrayPageItem);







    // Add LFO depth to oscillator tab
    /*resonancePotentiometerArrayPageItem.SetScaledRange(0, 255);
    resonancePotentiometerArrayPageItem.SetOutputValue(0.5f);
    resonancePotentiometerArrayPageItem.SetTitle("LFO MOD");
    oscillatorPotentiometerArrayPage.SetItem(11, &resonancePotentiometerArrayPageItem);*/

    // Add quick access tab to tab page
    quickTabPageItem.Init("QUICK", &quickPotentiometerArrayPage);
    tabPage.AddItem(&quickTabPageItem);
    quickPotentiometerArrayPage.SetUseQuickAlias(true);

    // Add existing items to quick view page
    quickPotentiometerArrayPage.SetItem(0, &noiseLevelPotentiometerArrayPageItem);

    // Add settings menu to tab page
    tabPage.SetMenuContent(&settingsListPage);

    // Add MIDI channel to settings page
    midiChannelSettingsListPageItem.Init("MIDI channel", &settingsListPage, 1, 16, 1);
    settingsListPage.AddItem(&midiChannelSettingsListPageItem);

    // Add knob mode to settings page
    knobModeSettingsListPageItem.Init("Knob mode", &settingsListPage);
    knobModeSettingsListPageItem.AddOption("Direct", PotentiometerArrayPageItem::DIRECT);
    knobModeSettingsListPageItem.AddOption("Catch", PotentiometerArrayPageItem::CATCH);
    settingsListPage.AddItem(&knobModeSettingsListPageItem);

    // Set home and current pages
    SetHomePage(&tabPage);
    SetCurrentPage(&tabPage);
}

ParameterSet UserInterface::GetParameters()
{
    ParameterSet parameterSet;
    parameterSet.osc1WaveShape = osc1WaveShapePotentiometerArrayPageItem.GetSelectedValue();
    parameterSet.osc1Detune = osc1DetunePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.osc2WaveShape = osc2WaveShapePotentiometerArrayPageItem.GetSelectedValue();
    parameterSet.osc2Detune = osc2DetunePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.voices = voicesPotentiometerArrayPageItem.GetDisplayValue();
    parameterSet.portamento = portamentoPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.osc2Transpose = osc2TransposePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.osc2Level = osc2LevelPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.pitchAttack = pitchAttackPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.pitchDecay = pitchDecayPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.pitchSustain = pitchSustainPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.pitchRelease = pitchReleasePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.noiseLevel = noiseLevelPotentiometerArrayPageItem.GetOutputValue();

    parameterSet.filterAttack = filterAttackPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.filterDecay = filterDecayPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.filterSustain = filterSustainPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.filterRelease = filterReleasePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.cutOff = cutOffPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.resonance = resonancePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.filterType = filterTypePotentiometerArrayPageItem.GetSelectedValue();
    parameterSet.filterEnvelopeAmount = filterEnvelopeAmountPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.ampAttack = ampAttackPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.ampDecay = ampDecayPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.ampSustain = ampSustainPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.ampRelease = ampReleasePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.pan = panPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.mixLevel = mixLevelPotentiometerArrayPageItem.GetOutputValue();

    parameterSet.lfoLevel = lfoLevelPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.lfoFrequency = lfoFrequencyPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.lfoWaveShape = lfoWaveShapePotentiometerArrayPageItem.GetSelectedValue();
    parameterSet.lfoTarget = lfoTargetPotentiometerArrayPageItem.GetSelectedValue();






    // parameterSet.note = noteSettingsPageItem.GetValue();
    parameterSet.midiChannel = (uint8_t)midiChannelSettingsListPageItem.GetValue();
    parameterSet.knobMode = (PotentiometerArrayPageItem::KnobMode)knobModeSettingsListPageItem.GetValue();
    return parameterSet;
}

void UserInterface::UpdateSettings(ParameterSet parameterSet)
{
    oscillatorPotentiometerArrayPage.SetKnobMode(parameterSet.knobMode);
    filterAmpPotentiometerArrayPage.SetKnobMode(parameterSet.knobMode);
}
