#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9

#include <string>
#include "daisysp.h"
#include "daisy_seed.h"
#include "ParameterSet.h"
#include "../../Presenters/TreeRoot.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Utilities/UiParameter.h"
#include "../../View/ListPageSsd1306I2cView.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tree::presenters;
using namespace developmentKit::library::uiFramework::tree::utilities;
using namespace developmentKit::library::uiFramework::tree::view;

class UserInterface : public TreeRoot
{
public:
    UserInterface();
    ~UserInterface() {}
    void Init();
    void Paint();
    ParameterSet GetParameters();

private:
    OledDisplay<SSD130xI2c128x64Driver> oledDisplay;
    ListPageSsd1306I2cView listPageView;
    ListPage homeListPage;
    NavigationPageItem oscillatorNavigationPageItem;
    NavigationPageItem adsrNavigationPageItem;
    ListPage oscillatorListPage;
    NavigationPageItem oscillatorBackPageItem;
    NumericSettingsPageItem levelSettingsPageItem;
    NumericSettingsPageItem noteSettingsPageItem;
    OptionsSettingsPageItem waveformSettingsPageItem;
    ListPage adsrListPage;
    NavigationPageItem adsrBackPageItem;
    NumericSettingsPageItem attackSettingsPageItem;
    NumericSettingsPageItem decaySettingsPageItem;
    NumericSettingsPageItem sustainSettingsPageItem;
    NumericSettingsPageItem releaseSettingsPageItem;
};

#endif


