#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9

#include <string>
#include "daisysp.h"
#include "daisy_seed.h"
#include "ParameterSet.h"
#include "../../Presenters/Root.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Utilities/UiParameter.h"
//#include "../../Views/ListPageSsd1306I2cView.h"
#include "../../Views/ListPageView.h"
#include "../../ViewAdapters/Ssd1306I2cViewAdapter.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tree;
using namespace developmentKit::library::uiFramework::tree::utilities;
using namespace developmentKit::library::uiFramework::tree::view;
using namespace developmentKit::library::uiFramework::tree::viewAdapters;

class UserInterface : public Root
{
public:
    UserInterface() {}
    ~UserInterface() {}
    void Init(OledDisplay<SSD130xI2c128x64Driver> *prmOledDisplay);
    ParameterSet GetParameters();

private:
    OledDisplay<SSD130xI2c128x64Driver> *oledDisplay;
    //ListPageSsd1306I2cView listPageView;
    ListPageView listPageView;
    Ssd1306I2cViewAdapter viewAdapter;
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


