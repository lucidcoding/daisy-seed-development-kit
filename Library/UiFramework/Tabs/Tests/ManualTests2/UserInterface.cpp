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
    tftDisplay.Init();
    potentiometerArrayPageIli9341View.SetSize(12, 22, 296, 210);

    oscLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscillatorPotentiometerArrayPage.SetItem(0, &oscLevelPotentiometerArrayPageItem);
    oscCoarseTunePotentiometerArrayPageItem.SetScaledRange(-127, 127);
    oscCoarseTunePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscillatorPotentiometerArrayPage.SetItem(1, &oscCoarseTunePotentiometerArrayPageItem);
    oscWaveShapePotentiometerArrayPageItem.SetScaledRange(0, 3);
    oscWaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    
    oscWaveShapePotentiometerArrayPageItem.SetOptions(oscWaveShapeOptions);
    oscillatorPotentiometerArrayPage.SetItem(2, &oscWaveShapePotentiometerArrayPageItem);
    oscillatorTabPageItem.Init("OSC", &oscillatorPotentiometerArrayPage);
    tabPage.AddItem(&oscillatorTabPageItem);

    envelopePotentiometerArrayPage.SetItem(0, &envAttackPotentiometerArrayPageItem);
    envelopePotentiometerArrayPage.SetItem(1, &envDecayPotentiometerArrayPageItem);
    envelopeTabPageItem.Init("ENV", &envelopePotentiometerArrayPage);
    tabPage.AddItem(&envelopeTabPageItem);

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
