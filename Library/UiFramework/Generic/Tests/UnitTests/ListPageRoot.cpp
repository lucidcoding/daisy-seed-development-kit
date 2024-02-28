#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "ParameterSet.h"
#include "ListPageRoot.h"
#include "../../Presenters/Root.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationListPageItem.h"
#include "../../Presenters/NumericSettingsListPageItem.h"
#include "../../Presenters/OptionsSettingsListPageItem.h"

using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tree::utilities;
using namespace developmentKit::library::uiFramework::tests::unitTests;

void ListPageRoot::Init()
{
    // Initialise home list page
    homeListPage.Init(&mockView);

    // Initialise ocillator list page
    oscillatorListPage.Init(&mockView);

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
    waveformSettingsPageItem.AddOption("Sin", 0);
    waveformSettingsPageItem.AddOption("Tri", 1);
    waveformSettingsPageItem.AddOption("Saw", 2);
    waveformSettingsPageItem.AddOption("Squ", 3);
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
    adsrListPage.Init(&mockView);

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

ParameterSet ListPageRoot::GetParameters()
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

