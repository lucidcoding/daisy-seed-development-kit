#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9

#include <string>
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Utilities/UiParameter.h"
#include "../../View/ListPageSsd1306I2cView.h"
#include "../../Display.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tree;
using namespace developmentKit::library::uiFramework::tree::utilities;
using namespace developmentKit::library::uiFramework::tree::view;

class UserInterface : public Display
{
public:
    UserInterface();
    ~UserInterface() {}
    void Init();
    /*void Increment();
    void Decrement();
    void Select();*/
    void Paint();

private:
    OledDisplay<SSD130xI2c128x64Driver> oledDisplay;
    ListPageSsd1306I2cView listPageView;
    ListPage homeListPage;
    NavigationPageItem oscillatorNavigationPageItem;
    ListPage oscillatorListPage;
    NavigationPageItem oscillatorBackPageItem;
    NumericSettingsPageItem levelSettingsPageItem;
    NumericSettingsPageItem noteSettingsPageItem;
    ListPage adsrListPage;
    NavigationPageItem adsrBackPageItem;
    NumericSettingsPageItem attackSettingsPageItem;
    NumericSettingsPageItem delaySettingsPageItem;
    NumericSettingsPageItem sustainSettingsPageItem;
    NumericSettingsPageItem releaseSettingsPageItem;
};

#endif


