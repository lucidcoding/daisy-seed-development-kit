#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "ParameterSet.h"
#include "../../Library/UiFramework/Presenters/Root.h"
#include "../../Library/UiFramework/Presenters/ListPage.h"
#include "../../Library/UiFramework/Presenters/NavigationListPageItem.h"
#include "../../Library/UiFramework/Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/NumericSettingsListPageItem.h"
#include "../../Library/UiFramework/Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/OptionsSettingsListPageItem.h"
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPage.h"
#include "../../Library/UiFramework/Presenters/PotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Presenters/TabPage.h"
#include "../../Library/UiFramework/Presenters/TabPageItem.h"
#include "../../Library/UiFramework/Views/ListPageView.h"
#include "../../Library/UiFramework/Views/PotentiometerArrayPageView.h"
#include "../../Library/UiFramework/Views/TabPageView.h"
#include "../../Library/UiFramework/ViewAdapters/Ili9341ViewAdapter.h"
#include "../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

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
    TabPageItem envelopeTabPageItem;
    PotentiometerArrayPage envelopePotentiometerArrayPage;
    NumericPotentiometerArrayPageItem envAttackPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem envDecayPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem envSustainPotentiometerArrayPageItem;
    NumericPotentiometerArrayPageItem envReleasePotentiometerArrayPageItem;
    TabPageItem quickTabPageItem;
    PotentiometerArrayPage quickPotentiometerArrayPage;
    ListPage settingsListPage;
    NumericSettingsListPageItem midiChannelSettingsListPageItem;
    OptionsSettingsListPageItem knobModeSettingsListPageItem;
};

#endif


