#include "UserInterface.h"
#include "daisysp.h"
#include "daisy_seed.h"
#include "ParameterSet.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationListPageItem.h"
#include "../../Presenters/OptionsSettingsListPageItem.h"
#include "../../ViewAdapters/Ssd1306I2cViewAdapter.h"
#include "../../ViewAdapters/Ili9341ViewAdapter.h"
#include "../../Views/ListPageView.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tree;
using namespace developmentKit::library::uiFramework::tree::view;
using namespace developmentKit::library::uiFramework::tree::viewAdapters;

void UserInterface::Init(UiDriver *prmUiDriver)
{
    // Initialise display
    uiDriver = prmUiDriver;
    uiDriver->Init();

    // Initialise view
    viewAdapter.Init(uiDriver);
    listPageView.Init(&viewAdapter, 0, 0, 320, 240);

    // Initialise home list page
    homeListPage.Init(&listPageView);

    // Initialise ocillator list page
    oscillatorListPage.Init(&listPageView);

    // Add ocillator navigation item to home page
    oscillatorNavigationPageItem.Init("Oscillator...", &oscillatorListPage, this);
    homeListPage.AddItem(&oscillatorNavigationPageItem);

    // Add back navigation item to oscillator page
    oscillatorBackPageItem.Init("Back...", &homeListPage, this);
    oscillatorListPage.AddItem(&oscillatorBackPageItem);

    // Add level settings to oscillator page
    levelSettingsPageItem.Init("Level", &oscillatorListPage, 0, 127, 16);
    oscillatorListPage.AddItem(&levelSettingsPageItem);

    // Add waveform settings to oscilator page
    waveformSettingsPageItem.Init("Waveform", &oscillatorListPage);
    waveformSettingsPageItem.AddOption("Sin", Oscillator::WAVE_SIN);
    waveformSettingsPageItem.AddOption("Tri", Oscillator::WAVE_TRI);
    waveformSettingsPageItem.AddOption("Saw", Oscillator::WAVE_SAW);
    waveformSettingsPageItem.AddOption("Squ", Oscillator::WAVE_SQUARE);
    oscillatorListPage.AddItem(&waveformSettingsPageItem);

    // Add note settings to oscillator page
    noteSettingsPageItem.Init("Note", &oscillatorListPage, 0, 127, 64);
    oscillatorListPage.AddItem(&noteSettingsPageItem);

    // Add envelope navigation item to home page
    adsrNavigationPageItem.Init("Envelope...", &adsrListPage, this);
    homeListPage.AddItem(&adsrNavigationPageItem);

    // Add back navigation item to envelope page
    adsrBackPageItem.Init("Back...", &homeListPage, this);
    adsrListPage.AddItem(&adsrBackPageItem);

    // Initialise envelope page
    adsrListPage.Init(&listPageView);

    // Add attack settings to envelope page
    attackSettingsPageItem.Init("Attack", &adsrListPage, 0, 127, 0);
    adsrListPage.AddItem(&attackSettingsPageItem);

    // Add attack settings to envelope page
    decaySettingsPageItem.Init("Decay", &adsrListPage, 0, 127, 32);
    adsrListPage.AddItem(&decaySettingsPageItem);

    // Add sustain settings to envelope page
    sustainSettingsPageItem.Init("Sustain", &adsrListPage, 0, 127, 16);
    adsrListPage.AddItem(&sustainSettingsPageItem);

    // Add release settings to envelope page
    releaseSettingsPageItem.Init("Release", &adsrListPage, 0, 127, 16);
    adsrListPage.AddItem(&releaseSettingsPageItem);

    // Set home and current pages
    SetHomePage(&homeListPage);
    SetCurrentPage(&homeListPage);
}

ParameterSet UserInterface::GetParameters()
{
    ParameterSet parameterSet;
    parameterSet.level = levelSettingsPageItem.GetScaledValue();
    parameterSet.waveform = waveformSettingsPageItem.GetValue();
    parameterSet.note = noteSettingsPageItem.GetValue();
    parameterSet.attack = attackSettingsPageItem.GetScaledValue();
    parameterSet.decay = decaySettingsPageItem.GetScaledValue();
    parameterSet.sustain = sustainSettingsPageItem.GetScaledValue();
    parameterSet.release = releaseSettingsPageItem.GetScaledValue();
    return parameterSet;
}
