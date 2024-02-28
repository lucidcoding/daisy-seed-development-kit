#pragma once
#ifndef TEST_TREE_ROOT_H
#define TEST_TREE_ROOT_H

#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "ParameterSet.h"
#include "../../Presenters/Root.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/NumericSettingsPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"

using namespace developmentKit::library::uiFramework::presenters;
using namespace developmentKit::library::uiFramework::tree::utilities;
using namespace developmentKit::library::uiFramework::tests::unitTests;

class ListPageRoot : public Root
{
public:
    ListPageRoot() {}
    ~ListPageRoot() {}
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


