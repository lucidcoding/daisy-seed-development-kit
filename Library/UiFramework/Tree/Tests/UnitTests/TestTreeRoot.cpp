#include "TestTreeRoot.h"
#include "ParameterSet.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"

using namespace developmentKit::library::uiFramework::tree;
using namespace developmentKit::library::uiFramework::tree::view;

TestTreeRoot::TestTreeRoot() : homeListPage(&mockView),
                               oscillatorListPage(&mockView),
                               adsrListPage(&mockView)
{
}

void TestTreeRoot::Init()
{
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

ParameterSet TestTreeRoot::GetParameters()
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

void TestTreeRoot::Paint()
{
    currentPage->Paint();
}
