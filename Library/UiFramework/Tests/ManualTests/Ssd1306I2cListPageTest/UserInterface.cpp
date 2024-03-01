#include "UserInterface.h"
#include "daisysp.h"
#include "daisy_seed.h"
#include "ParameterSet.h"
#include "../../../Presenters/ListPage.h"
#include "../../../Presenters/NavigationListPageItem.h"
#include "../../../Presenters/OptionsSettingsListPageItem.h"
#include "../../../ViewAdapters/Ssd1306I2cViewAdapter.h"
#include "../../../Views/ListPageView.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::views;
using namespace developmentKit::library::uiFramework::viewAdapters;

void UserInterface::Init(OledDisplay<SSD130xI2c128x64Driver> *prmOledDisplay)
{
    // Initialise display
    oledDisplay = prmOledDisplay;
    OledDisplay<SSD130xI2c128x64Driver>::Config disp_cfg;
    disp_cfg.driver_config.transport_config.i2c_address = 0x3C;
    disp_cfg.driver_config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    disp_cfg.driver_config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
    disp_cfg.driver_config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, PIN_I2C_SCL};
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, PIN_I2C_SDA};
    oledDisplay->Init(disp_cfg);

    // Initialise view
    viewAdapter.Init(oledDisplay);
    listPageView.Init(&viewAdapter, 0, 0, 128, 64);

    // Initialise home list page
    homeListPage.Init(&listPageView);

    // Initialise ocillator list page
    oscillatorListPage.Init(&listPageView);

    // Add ocillator navigation item to home page
    oscillatorNavigationListPageItem.Init("Oscillator...", &oscillatorListPage, this);
    homeListPage.AddItem(&oscillatorNavigationListPageItem);

    // Add back navigation item to oscillator page
    oscillatorBackListPageItem.Init("Back...", &homeListPage, this);
    oscillatorListPage.AddItem(&oscillatorBackListPageItem);

    // Add level settings to oscillator page
    levelSettingsListPageItem.Init("Level", &oscillatorListPage, 0, 127, 16);
    oscillatorListPage.AddItem(&levelSettingsListPageItem);

    // Add waveform settings to oscilator page
    waveformSettingsListPageItem.Init("Waveform", &oscillatorListPage);
    waveformSettingsListPageItem.AddOption("Sin", Oscillator::WAVE_SIN);
    waveformSettingsListPageItem.AddOption("Tri", Oscillator::WAVE_TRI);
    waveformSettingsListPageItem.AddOption("Saw", Oscillator::WAVE_SAW);
    waveformSettingsListPageItem.AddOption("Squ", Oscillator::WAVE_SQUARE);
    oscillatorListPage.AddItem(&waveformSettingsListPageItem);

    // Add note settings to oscillator page
    noteSettingsListPageItem.Init("Note", &oscillatorListPage, 0, 127, 64);
    oscillatorListPage.AddItem(&noteSettingsListPageItem);

    // Add envelope navigation item to home page
    adsrNavigationListPageItem.Init("Envelope...", &adsrListPage, this);
    homeListPage.AddItem(&adsrNavigationListPageItem);

    // Add back navigation item to envelope page
    adsrBackListPageItem.Init("Back...", &homeListPage, this);
    adsrListPage.AddItem(&adsrBackListPageItem);

    // Initialise envelope page
    adsrListPage.Init(&listPageView);

    // Add attack settings to envelope page
    attackSettingsListPageItem.Init("Attack", &adsrListPage, 0, 127, 0);
    adsrListPage.AddItem(&attackSettingsListPageItem);

    // Add attack settings to envelope page
    decaySettingsListPageItem.Init("Decay", &adsrListPage, 0, 127, 32);
    adsrListPage.AddItem(&decaySettingsListPageItem);

    // Add sustain settings to envelope page
    sustainSettingsListPageItem.Init("Sustain", &adsrListPage, 0, 127, 16);
    adsrListPage.AddItem(&sustainSettingsListPageItem);

    // Add release settings to envelope page
    releaseSettingsListPageItem.Init("Release", &adsrListPage, 0, 127, 16);
    adsrListPage.AddItem(&releaseSettingsListPageItem);

    // Set home and current pages
    SetHomePage(&homeListPage);
    SetCurrentPage(&homeListPage);
}

ParameterSet UserInterface::GetParameters()
{
    ParameterSet parameterSet;
    parameterSet.level = levelSettingsListPageItem.GetScaledValue();
    parameterSet.waveform = waveformSettingsListPageItem.GetValue();
    parameterSet.note = noteSettingsListPageItem.GetValue();
    parameterSet.attack = attackSettingsListPageItem.GetScaledValue();
    parameterSet.decay = decaySettingsListPageItem.GetScaledValue();
    parameterSet.sustain = sustainSettingsListPageItem.GetScaledValue();
    parameterSet.release = releaseSettingsListPageItem.GetScaledValue();
    return parameterSet;
}
