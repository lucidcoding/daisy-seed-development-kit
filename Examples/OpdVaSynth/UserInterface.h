#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Library/UiFramework/Tabs/Presenters/ListPage.h"
#include "../../Library/UiFramework/Tabs/Presenters/PotentiometerArrayPage.h"
#include "../../Library/UiFramework/Tabs/Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Tabs/Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../Library/UiFramework/Tabs/Presenters/NavigationPageItem.h"
#include "../../Library/UiFramework/Tabs/Presenters/OptionsSettingsPageItem.h"
#include "../../Library/UiFramework/Tabs/Presenters/TabPageItem.h"
#include "../../Library/UiFramework/Tabs/Presenters/TabPage.h"
#include "../../Library/UiFramework/Tabs/Utilities/UiParameter.h"
#include "../../Library/UiFramework/Tabs/View/PotentiometerArrayPageIli9341View.h"
#include "../../Library/UiFramework/Tabs/View/TabPageIli9341View.h"
#include "../../Library/UiFramework/Tabs/View/ListPageIli9341View.h"
#include "../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::utilities;
using namespace developmentKit::library::uiFramework::tabs::view;

class UserInterface
{
public:
    UserInterface();
    ~UserInterface() {}
    void Init();
    void Increment();
    void Decrement();
    void Select();
    void SetPotentiometerValues(float *values);
    void Paint();

private:
    UiDriver tftDisplay;
    TabPageIli9341View tabPageView;
    PotentiometerArrayPageIli9341View potentiometerArrayPageIli9341View;
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


