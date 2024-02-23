#pragma once
#ifndef TEST_TREE_ROOT_H
#define TEST_TREE_ROOT_H

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9

#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "ParameterSet.h"
#include "../../../../../ThirdParty/catch.hpp"
#include "../../Presenters/TreeRoot.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Utilities/UiParameter.h"
#include "../../Utilities/UiParameterProvider.h"

using namespace developmentKit::library::uiFramework::tree::presenters;
using namespace developmentKit::library::uiFramework::tree::tests;
using namespace developmentKit::library::uiFramework::tree::utilities;

class TestTreeRoot : public TreeRoot
{
public:
    TestTreeRoot();
    ~TestTreeRoot() {}
    void Init();
    void Paint();
    ParameterSet GetParameters();
    MockView mockView;
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

private:
};

#endif


