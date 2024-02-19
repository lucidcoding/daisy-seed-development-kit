#include "UserInterface.h"
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../View/ListPageSsd1306I2cView.h"
#include "../../View/ListPageIli9341View.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/PotentiometerArrayPage.h"
#include "../../View/PotentiometerArrayPageIli9341View.h"
#include "../../View/TabPageIli9341View.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/TabPageItem.h"
#include "../../Presenters/TabPage.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"
#include "../../../../Utilities/Scaling/FloatToIntScaler.h"

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::utilities;
using namespace developmentKit::library::uiFramework::tabs::view;

UserInterface::UserInterface() : 
    tabPageView(&tftDisplay),
    potentiometerArrayPageIli9341View(&tftDisplay),
    tabPage(&tabPageView),
    oscillatorPotentiometerArrayPage(&potentiometerArrayPageIli9341View),
    envelopePotentiometerArrayPage(&potentiometerArrayPageIli9341View)
{
}

void UserInterface::Init()
{
    standardSignedScaler.Init(-127, 127, FloatToIntScaler::Curve::LINEAR);
    standardUnsignedScaler.Init(0, 255, FloatToIntScaler::Curve::LINEAR);
    signedScaler24.Init(-24, 24, FloatToIntScaler::Curve::LINEAR);
    optionsScaler4Pole.Init(0, 3, FloatToIntScaler::Curve::LINEAR);

    tftDisplay.Init();
    potentiometerArrayPageIli9341View.SetSize(12, 22, 296, 210);

    oscLevelPotentiometerArrayPageItem.SetScaler(&standardSignedScaler);
    oscillatorPotentiometerArrayPage.SetItem(0, &oscLevelPotentiometerArrayPageItem);
    oscCoarseTunePotentiometerArrayPageItem.SetScaler(&signedScaler24);
    oscillatorPotentiometerArrayPage.SetItem(1, &oscCoarseTunePotentiometerArrayPageItem);
    oscWaveShapePotentiometerArrayPageItem.SetScaler(&optionsScaler4Pole);
    oscillatorPotentiometerArrayPage.SetItem(2, &oscWaveShapePotentiometerArrayPageItem);
    oscillatorTabPageItem.Init("OSC", &oscillatorPotentiometerArrayPage);
    tabPage.AddItem(&oscillatorTabPageItem);

    envelopePotentiometerArrayPage.SetItem(0, &envAttackPotentiometerArrayPageItem);
    envelopePotentiometerArrayPage.SetItem(1, &envDecayPotentiometerArrayPageItem);
    envelopeTabPageItem.Init("ENV", &envelopePotentiometerArrayPage);
    tabPage.AddItem(&envelopeTabPageItem);

    //tabPage.AddItem(new TabPageItem("PAGE 0", &potentiometerArrayPage1));
}

void UserInterface::Increment()
{
    tabPage.Increment();
}

void UserInterface::Decrement()
{
    tabPage.Decrement();
}

void UserInterface::Select()
{
    tabPage.Select();
}

void UserInterface::SetPotentiometerValue(uint8_t index, float value)
{
}

void UserInterface::SetPotentiometerValues(float *values)
{
    tabPage.SetPotentiometerValues(values);
}

void UserInterface::Paint()
{
    tabPage.Paint();
    tftDisplay.Update();
}
