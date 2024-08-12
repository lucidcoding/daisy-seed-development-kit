#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "ParameterSet.h"
#include "../../../Presenters/Root.h"
#include "../../../Presenters/ListPage.h"
#include "../../../Presenters/NavigationListPageItem.h"
#include "../../../Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../../Presenters/NumericSettingsListPageItem.h"
#include "../../../Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../../Presenters/OptionsSettingsListPageItem.h"
#include "../../../Presenters/PotentiometerArrayPage.h"
#include "../../../Presenters/PotentiometerArrayPageItem.h"
#include "../../../Presenters/TabPage.h"
#include "../../../Presenters/TabPageItem.h"
#include "../../../Views/ListPageView.h"
#include "../../../Views/PotentiometerArrayPageView.h"
#include "../../../Views/TabPageView.h"
#include "../../../ViewAdapters/Ili9341ViewAdapter.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

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
    void UpdateSettings(ParameterSet parameterSet);

private:
    UiDriver *uiDriver;
    ListPageView listPageView;
    TabPageView tabPageView;
    PotentiometerArrayPageView potentiometerArrayPageView;
    Ili9341ViewAdapter viewAdapter;
    TabPage tabPage;
    TabPageItem oscillatorTabPageItem;
    PotentiometerArrayPage oscillatorPotentiometerArrayPage;
    OptionsPotentiometerArrayPageItem oscWaveShapePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscLevelPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscOctavePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscSemitonePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscFineTunePotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscPulseWidthPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscPulseWidthModulationPotentiometerArrayPageItem;
    OptionsPotentiometerArrayPageItem oscSyncPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem oscLfoDepthPotentiometerArrayPageItem;
    TabPageItem filterAmpTabPageItem;
    PotentiometerArrayPage filterAmpPotentiometerArrayPage;
    NumericPotentiometerArrayPageItem filterAttackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterDecayPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterSustainPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem filterReleasePotentiometerArrayPageItem;
    TabPageItem quickTabPageItem;
    PotentiometerArrayPage quickPotentiometerArrayPage;
    ListPage settingsListPage;
    NumericSettingsListPageItem midiChannelSettingsListPageItem;
    OptionsSettingsListPageItem knobModeSettingsListPageItem;
};

#endif


