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
#include "../../Presenters/NavigationListPageItem.h"
#include "../../Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../Presenters/PotentiometerArrayPage.h"
#include "../../Presenters/PotentiometerArrayPageItem.h"
#include "../../Presenters/TabPage.h"
#include "../../Presenters/TabPageItem.h"
#include "../../Utilities/UiParameter.h"
#include "../../Views/ListPageView.h"
#include "../../Views/PotentiometerArrayPageView.h"
#include "../../Views/TabPageView.h"
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
};

#endif


