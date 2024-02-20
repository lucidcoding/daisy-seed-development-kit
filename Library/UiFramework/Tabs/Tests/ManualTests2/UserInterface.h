#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../View/ListPageSsd1306I2cView.h"
#include "../../View/ListPageIli9341View.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/PotentiometerArrayPage.h"
#include "../../Presenters/ContinuousPotentiometerArrayPageItem.h"
#include "../../View/PotentiometerArrayPageIli9341View.h"
#include "../../View/TabPageIli9341View.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/TabPageItem.h"
#include "../../Presenters/TabPage.h"
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
    void SetPotentiometerValue(uint8_t index, float value);
    void SetPotentiometerValues(float *values);
    void Paint();

private:
    UiDriver tftDisplay;
    TabPageIli9341View tabPageView;
    PotentiometerArrayPageIli9341View potentiometerArrayPageIli9341View;
    TabPage tabPage;
    TabPageItem oscillatorTabPageItem;
    PotentiometerArrayPage oscillatorPotentiometerArrayPage;
    ContinuousPotentiometerArrayPageItem oscLevelPotentiometerArrayPageItem;
    ContinuousPotentiometerArrayPageItem oscCoarseTunePotentiometerArrayPageItem;
    ContinuousPotentiometerArrayPageItem oscWaveShapePotentiometerArrayPageItem;
    TabPageItem envelopeTabPageItem;
    PotentiometerArrayPage envelopePotentiometerArrayPage;
    ContinuousPotentiometerArrayPageItem envAttackPotentiometerArrayPageItem;
    ContinuousPotentiometerArrayPageItem envDecayPotentiometerArrayPageItem;

    string oscWaveShapeOptions[4] = {"SIN", "TRI", "SAW", "SQU"};
};

#endif


