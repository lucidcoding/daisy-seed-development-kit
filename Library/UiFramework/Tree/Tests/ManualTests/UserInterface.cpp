#include "UserInterface.h"
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../View/ListPageSsd1306I2cView.h"
#include "../../View/ListPageIli9341View.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tree;
using namespace developmentKit::library::uiFramework::tree::view;

UserInterface::UserInterface() : listPageView(&oledDisplay),
                                 homeListPage(&listPageView),
                                 oscillatorListPage(&listPageView),
                                 adsrListPage(&listPageView)
{
}

void UserInterface::Init()
{
    // oledDisplay.Init();
    OledDisplay<SSD130xI2c128x64Driver>::Config disp_cfg;
    disp_cfg.driver_config.transport_config.i2c_address = 0x3C;
    disp_cfg.driver_config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    disp_cfg.driver_config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
    disp_cfg.driver_config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, PIN_I2C_SCL};
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, PIN_I2C_SDA};
    oledDisplay.Init(disp_cfg);

    // Add ocillator navigation item to home page
    oscillatorNavigationPageItem.Init("Oscillator...", &oscillatorListPage, this);
    homeListPage.AddItem(&oscillatorNavigationPageItem);

    // Add back navigation item to oscillator page
    oscillatorBackPageItem.Init("Back...", &homeListPage, this);
    oscillatorListPage.AddItem(&oscillatorBackPageItem);

    // Add level settings to oscillator page
    levelSettingsPageItem.Init("Level", &oscillatorListPage, 0, 127, 16);
    oscillatorListPage.AddItem(&levelSettingsPageItem);

    // Add note settings to oscillator page
    noteSettingsPageItem.Init("Note", &oscillatorListPage, 0, 127, 64);
    oscillatorListPage.AddItem(&noteSettingsPageItem);

    SetHomePage(&homeListPage);
    SetCurrentPage(&homeListPage);
}

void UserInterface::Paint()
{
    currentPage->Paint();
    oledDisplay.Update();
}
