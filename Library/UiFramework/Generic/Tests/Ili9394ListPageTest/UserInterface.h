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
#include "../../ViewAdapters/Ili9341ViewAdapter.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

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
    void Init(UiDriver *prmUiDriver);
    ParameterSet GetParameters();

private:
    UiDriver *uiDriver;
    //ListPageSsd1306I2cView listPageView;
    ListPageView listPageView;
    Ili9341ViewAdapter viewAdapter;
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


