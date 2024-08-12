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
    quickPotentiometerArrayPage.Init(&potentiometerArrayPageView);
    settingsListPage.Init(&listPageView);

    // Add oscillator tab to tab page
    oscillatorTabPageItem.Init("OSC", &oscillatorPotentiometerArrayPage);
    tabPage.AddItem(&oscillatorTabPageItem);

    // Add oscillator 1 wave shape to oscillator tab
    osc1WaveShapePotentiometerArrayPageItem.AddOption("SIN", 0);
    osc1WaveShapePotentiometerArrayPageItem.AddOption("TRI", 1);
    osc1WaveShapePotentiometerArrayPageItem.AddOption("SAW", 2);
    osc1WaveShapePotentiometerArrayPageItem.AddOption("SQU", 4);
    osc1WaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc1WaveShapePotentiometerArrayPageItem.SetTitle("OSC 1 WAVE");
    oscillatorPotentiometerArrayPage.SetItem(0, &osc1WaveShapePotentiometerArrayPageItem);

    // Add oscillator 1 detune to oscillator tab
    osc1DetunePotentiometerArrayPageItem.SetScaledRange(0, 255);
    osc1DetunePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc1DetunePotentiometerArrayPageItem.SetTitle("OSC 1 DETUNE");
    oscillatorPotentiometerArrayPage.SetItem(1, &osc1DetunePotentiometerArrayPageItem);

    // Add oscillator 2 wave shape to oscillator tab
    osc2WaveShapePotentiometerArrayPageItem.AddOption("SIN", 0);
    osc2WaveShapePotentiometerArrayPageItem.AddOption("TRI", 1);
    osc2WaveShapePotentiometerArrayPageItem.AddOption("SAW", 2);
    osc2WaveShapePotentiometerArrayPageItem.AddOption("SQU", 4);
    osc2WaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2WaveShapePotentiometerArrayPageItem.SetTitle("OSC WAVE");
    oscillatorPotentiometerArrayPage.SetItem(2, &osc2WaveShapePotentiometerArrayPageItem);

    // Add oscillator 2 coarse tune to oscillator tab
    osc2TransposePotentiometerArrayPageItem.SetScaledRange(-12, 12);
    osc2TransposePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2TransposePotentiometerArrayPageItem.SetTitle("OSC 2 COARSE");
    oscillatorPotentiometerArrayPage.SetItem(3, &osc2TransposePotentiometerArrayPageItem);

    // Add oscillator 2 coarse tune to oscillator tab
    osc2DetunePotentiometerArrayPageItem.SetScaledRange(0, 255);
    osc2DetunePotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2DetunePotentiometerArrayPageItem.SetTitle("OSC 2 FINE");
    oscillatorPotentiometerArrayPage.SetItem(4, &osc2DetunePotentiometerArrayPageItem);

    // Add oscillator 2 coarse tune to oscillator tab
    osc2LevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    osc2LevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    osc2LevelPotentiometerArrayPageItem.SetTitle("OSC 2 LEVEL");
    oscillatorPotentiometerArrayPage.SetItem(5, &osc2LevelPotentiometerArrayPageItem);

    // Add noise level to oscillator tab
    noiseLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    noiseLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    noiseLevelPotentiometerArrayPageItem.SetTitle("NOISE LEVEL");
    noiseLevelPotentiometerArrayPageItem.SetQuickAlias("OSC:PW");
    oscillatorPotentiometerArrayPage.SetItem(6, &noiseLevelPotentiometerArrayPageItem);

    // Add portamento to oscillator tab
    portamentoPotentiometerArrayPageItem.SetScaledRange(0, 255);
    portamentoPotentiometerArrayPageItem.SetOutputValue(0.5f);
    portamentoPotentiometerArrayPageItem.SetTitle("PWM");
    oscillatorPotentiometerArrayPage.SetItem(7, &portamentoPotentiometerArrayPageItem);

    // Add cut off to oscillator tab
    cutOffPotentiometerArrayPageItem.SetScaledRange(0, 255);
    cutOffPotentiometerArrayPageItem.SetOutputValue(0.5f);
    cutOffPotentiometerArrayPageItem.SetTitle("CUT OFF");
    oscillatorPotentiometerArrayPage.SetItem(8, &cutOffPotentiometerArrayPageItem);

    // Add resonance to oscillator tab
    resonancePotentiometerArrayPageItem.SetScaledRange(0, 255);
    resonancePotentiometerArrayPageItem.SetOutputValue(0.5f);
    resonancePotentiometerArrayPageItem.SetTitle("RESONANCE");
    oscillatorPotentiometerArrayPage.SetItem(9, &resonancePotentiometerArrayPageItem);

    // Add wave sync to oscillator tab
    oscSyncPotentiometerArrayPageItem.AddOption("OFF", 0);
    oscSyncPotentiometerArrayPageItem.AddOption("SOFT", 1);
    oscSyncPotentiometerArrayPageItem.AddOption("HARD", 2);
    oscSyncPotentiometerArrayPageItem.SetTitle("SYNC");
    oscillatorPotentiometerArrayPage.SetItem(10, &oscSyncPotentiometerArrayPageItem);

    // Add LFO depth to oscillator tab
    /*resonancePotentiometerArrayPageItem.SetScaledRange(0, 255);
    resonancePotentiometerArrayPageItem.SetOutputValue(0.5f);
    resonancePotentiometerArrayPageItem.SetTitle("LFO MOD");
    oscillatorPotentiometerArrayPage.SetItem(11, &resonancePotentiometerArrayPageItem);*/

    // Add envelope tab to tab page
    filterAmpTabPageItem.Init("ENV", &filterAmpPotentiometerArrayPage);
    tabPage.AddItem(&filterAmpTabPageItem);

    // Add attack to envelope tab
    filterAttackPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterAttackPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterAttackPotentiometerArrayPageItem.SetTitle("ATTACK");
    filterAmpPotentiometerArrayPage.SetItem(0, &filterAttackPotentiometerArrayPageItem);

    // Add decay to envelope tab
    filterDecayPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterDecayPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterDecayPotentiometerArrayPageItem.SetTitle("DECAY");
    filterAmpPotentiometerArrayPage.SetItem(1, &filterDecayPotentiometerArrayPageItem);

    // Add sustain to envelope tab
    filterSustainPotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterSustainPotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterSustainPotentiometerArrayPageItem.SetTitle("SUSTAIN");
    filterAmpPotentiometerArrayPage.SetItem(2, &filterSustainPotentiometerArrayPageItem);

    // Add release to envelope tab
    filterReleasePotentiometerArrayPageItem.SetScaledRange(0, 255);
    filterReleasePotentiometerArrayPageItem.SetOutputValue(0.5f);
    filterReleasePotentiometerArrayPageItem.SetTitle("RELEASE");
    filterAmpPotentiometerArrayPage.SetItem(3, &filterReleasePotentiometerArrayPageItem);

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
    parameterSet.osc2Transpose = osc2TransposePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.osc2Detune = osc2DetunePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.osc2Level = osc2LevelPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.noiseLevel = noiseLevelPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.portamento = portamentoPotentiometerArrayPageItem.GetOutputValue();

    parameterSet.cutOff = cutOffPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.resonance = resonancePotentiometerArrayPageItem.GetOutputValue();

    parameterSet.level = osc1DetunePotentiometerArrayPageItem.GetOutputValue();
    //parameterSet.note = noteSettingsPageItem.GetValue();
    parameterSet.attack = filterAttackPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.decay = filterDecayPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.sustain = filterSustainPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.release = filterReleasePotentiometerArrayPageItem.GetOutputValue();
    parameterSet.midiChannel = (uint8_t)midiChannelSettingsListPageItem.GetValue();
    parameterSet.knobMode = (PotentiometerArrayPageItem::KnobMode)knobModeSettingsListPageItem.GetValue();
    return parameterSet;
}

void UserInterface::UpdateSettings(ParameterSet parameterSet)
{
    oscillatorPotentiometerArrayPage.SetKnobMode(parameterSet.knobMode);
    filterAmpPotentiometerArrayPage.SetKnobMode(parameterSet.knobMode);
}
