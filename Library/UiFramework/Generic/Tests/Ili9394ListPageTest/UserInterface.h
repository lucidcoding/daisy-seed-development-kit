#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9

#include "daisysp.h"
#include "daisy_seed.h"
#include "ParameterSet.h"
#include "../../Presenters/Root.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationListPageItem.h"
#include "../../Presenters/OptionsSettingsListPageItem.h"
#include "../../Presenters/NumericSettingsListPageItem.h"
#include "../../ViewAdapters/Ili9341ViewAdapter.h"
#include "../../Views/ListPageView.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::views;
using namespace developmentKit::library::uiFramework::viewAdapters;

class UserInterface : public Root
{
public:
    UserInterface() {}
    ~UserInterface() {}
    void Init(UiDriver *prmUiDriver);
    ParameterSet GetParameters();

private:
    UiDriver *uiDriver;
    // ListPageSsd1306I2cView listPageView;
    ListPageView listPageView;
    Ili9341ViewAdapter viewAdapter;
    ListPage homeListPage;
    NavigationListPageItem oscillatorNavigationPageItem;
    NavigationListPageItem adsrNavigationPageItem;
    ListPage oscillatorListPage;
    NavigationListPageItem oscillatorBackPageItem;
    NumericSettingsListPageItem levelSettingsPageItem;
    NumericSettingsListPageItem noteSettingsPageItem;
    OptionsSettingsListPageItem waveformSettingsPageItem;
    ListPage adsrListPage;
    NavigationListPageItem adsrBackPageItem;
    NumericSettingsListPageItem attackSettingsPageItem;
    NumericSettingsListPageItem decaySettingsPageItem;
    NumericSettingsListPageItem sustainSettingsPageItem;
    NumericSettingsListPageItem releaseSettingsPageItem;
};

#endif
