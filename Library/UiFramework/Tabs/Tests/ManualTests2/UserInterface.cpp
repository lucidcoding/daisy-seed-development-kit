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
    oscillatorPotentiometerArrayPage(&potentiometerArrayPageIli9341View)
{
}

void UserInterface::Init()
{
    tftDisplay.Init();
    potentiometerArrayPageIli9341View.SetSize(12, 22, 296, 210);
    oscillatorTabPageItem.Init("OSC", &oscillatorPotentiometerArrayPage);
    tabPage.AddItem(&oscillatorTabPageItem);
    //tabPage.AddItem(new TabPageItem("PAGE 0", &potentiometerArrayPage1));
}

void UserInterface::Increment()
{
}

void UserInterface::Decrement()
{
}

void UserInterface::Select()
{
}

void UserInterface::SetPotentiometerValue(uint8_t index, float value)
{
}

void UserInterface::SetPotentiometerValues(float *values)
{
}

void UserInterface::Paint()
{
    tabPage.Paint();
    tftDisplay.Update();
}
