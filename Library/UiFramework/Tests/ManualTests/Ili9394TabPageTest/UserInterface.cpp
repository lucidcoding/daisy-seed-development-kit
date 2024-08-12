#include "UserInterface.h"
#include "ParameterSet.h"
#include "../../../Presenters/ListPage.h"
#include "../../../Presenters/NavigationListPageItem.h"
#include "../../../Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../../Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../../Presenters/PotentiometerArrayPage.h"
#include "../../../Presenters/PotentiometerArrayPageItem.h"
#include "../../../Presenters/TabPage.h"
#include "../../../Presenters/TabPageItem.h"
#include "../../../Views/ListPageView.h"
#include "../../../Views/PotentiometerArrayPageView.h"
#include "../../../Views/TabPageView.h"
#include "../../../ViewAdapters/Ili9341ViewAdapter.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

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

    // Add wave shape to oscillator tab
    oscWaveShapePotentiometerArrayPageItem.AddOption("SIN", 0);
    oscWaveShapePotentiometerArrayPageItem.AddOption("TRI", 1);
    oscWaveShapePotentiometerArrayPageItem.AddOption("SAW", 2);
    oscWaveShapePotentiometerArrayPageItem.AddOption("SQU", 4);
    oscWaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscWaveShapePotentiometerArrayPageItem.SetTitle("WAVEFORM");
    oscillatorPotentiometerArrayPage.SetItem(0, &oscWaveShapePotentiometerArrayPageItem);

    // Add level to oscillator tab
    oscLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscLevelPotentiometerArrayPageItem.SetTitle("LEVEL");
    oscillatorPotentiometerArrayPage.SetItem(1, &oscLevelPotentiometerArrayPageItem);

    // Add octave to oscillator tab
    oscOctavePotentiometerArrayPageItem.SetScaledRange(0, 3);
    oscOctavePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscOctavePotentiometerArrayPageItem.SetTitle("OCTAVE");
    oscillatorPotentiometerArrayPage.SetItem(2, &oscOctavePotentiometerArrayPageItem);

    // Add semitone to oscillator tab
    oscSemitonePotentiometerArrayPageItem.SetScaledRange(-12, 12);
    oscSemitonePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscSemitonePotentiometerArrayPageItem.SetTitle("SEMITONE");
    oscillatorPotentiometerArrayPage.SetItem(3, &oscSemitonePotentiometerArrayPageItem);

    // Add pulse width to oscillator tab
    oscPulseWidthPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscPulseWidthPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscPulseWidthPotentiometerArrayPageItem.SetTitle("PW");
    oscPulseWidthPotentiometerArrayPageItem.SetQuickAlias("OSC:PW");
    oscillatorPotentiometerArrayPage.SetItem(4, &oscPulseWidthPotentiometerArrayPageItem);

    // Add pulse width modulation to oscillator tab
    oscPulseWidthModulationPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscPulseWidthModulationPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscPulseWidthModulationPotentiometerArrayPageItem.SetTitle("PWM");
    oscillatorPotentiometerArrayPage.SetItem(5, &oscPulseWidthModulationPotentiometerArrayPageItem);

    // Add wave sync to oscillator tab
    oscSyncPotentiometerArrayPageItem.AddOption("OFF", 0);
    oscSyncPotentiometerArrayPageItem.AddOption("SOFT", 1);
    oscSyncPotentiometerArrayPageItem.AddOption("HARD", 2);
    oscSyncPotentiometerArrayPageItem.SetTitle("SYNC");
    oscillatorPotentiometerArrayPage.SetItem(6, &oscSyncPotentiometerArrayPageItem);

    // Add LFO depth to oscillator tab
    oscLfoDepthPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscLfoDepthPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscLfoDepthPotentiometerArrayPageItem.SetTitle("LFO MOD");
    oscillatorPotentiometerArrayPage.SetItem(7, &oscLfoDepthPotentiometerArrayPageItem);

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
    quickPotentiometerArrayPage.SetItem(0, &oscPulseWidthPotentiometerArrayPageItem);

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
    parameterSet.level = oscLevelPotentiometerArrayPageItem.GetOutputValue();
    parameterSet.waveform = oscWaveShapePotentiometerArrayPageItem.GetSelectedValue();
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
