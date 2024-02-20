#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/PotentiometerArrayPage.h"
#include "../../Presenters/NumericPotentiometerArrayPageItem.h"
#include "../../Presenters/OptionsPotentiometerArrayPageItem.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/TabPageItem.h"
#include "../../Presenters/TabPage.h"
#include "../../Utilities/UiParameter.h"
#include "../../View/PotentiometerArrayPageIli9341View.h"
#include "../../View/TabPageIli9341View.h"
#include "../../View/ListPageIli9341View.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

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


