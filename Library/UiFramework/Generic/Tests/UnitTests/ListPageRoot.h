#pragma once
#ifndef TEST_TREE_ROOT_H
#define TEST_TREE_ROOT_H

#include "MockUiParameterProvider.h"
#include "MockView.h"
#include "ParameterSet.h"
#include "../../Presenters/Root.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/NavigationListPageItem.h"
#include "../../Presenters/NumericSettingsListPageItem.h"
#include "../../Presenters/OptionsSettingsListPageItem.h"

namespace developmentKit::library::uiFramework::tests::unitTests
{
    using namespace developmentKit::library::uiFramework::presenters;
    using namespace developmentKit::library::uiFramework::tree::utilities;

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

    private:
    };
}

#endif
