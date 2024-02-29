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
#include "../../Views/ListPageView.h"
#include "../../ViewAdapters/Ssd1306I2cViewAdapter.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tree;
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
    ListPageView listPageView;
    Ssd1306I2cViewAdapter viewAdapter;
    ListPage homeListPage;
    NavigationListPageItem oscillatorNavigationListPageItem;
    NavigationListPageItem adsrNavigationListPageItem;
    ListPage oscillatorListPage;
    NavigationListPageItem oscillatorBackListPageItem;
    NumericSettingsListPageItem levelSettingsListPageItem;
    NumericSettingsListPageItem noteSettingsListPageItem;
    OptionsSettingsListPageItem waveformSettingsListPageItem;
    ListPage adsrListPage;
    NavigationListPageItem adsrBackListPageItem;
    NumericSettingsListPageItem attackSettingsListPageItem;
    NumericSettingsListPageItem decaySettingsListPageItem;
    NumericSettingsListPageItem sustainSettingsListPageItem;
    NumericSettingsListPageItem releaseSettingsListPageItem;
};

#endif


