#include "UserInterface.h"
#include "daisysp.h"
#include "daisy_seed.h"
#include "../../View/ListPageSsd1306I2cView.h"
#include "../../View/ListPageIli9341View.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/PotentiometerArrayPage.h"
#include "../../Presenters/NumericPotentiometerArrayPageItem.h"
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

    // Add oscillator tab to tab page
    oscillatorTabPageItem.Init("OSC", &oscillatorPotentiometerArrayPage);
    tabPage.AddItem(&oscillatorTabPageItem);

    // Add wave shape to oscillator tab
    oscWaveShapePotentiometerArrayPageItem.AddOption("SIN", 0);
    oscWaveShapePotentiometerArrayPageItem.AddOption("TRI", 1);
    oscWaveShapePotentiometerArrayPageItem.AddOption("SAW", 2);
    oscWaveShapePotentiometerArrayPageItem.AddOption("SQU", 3);
    oscWaveShapePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscWaveShapePotentiometerArrayPageItem.SetTitle("WAVEFORM");
    oscillatorPotentiometerArrayPage.SetItem(0, &oscWaveShapePotentiometerArrayPageItem);

    // Add level to oscillator tab
    oscLevelPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscLevelPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscLevelPotentiometerArrayPageItem.SetTitle("LEVEL");
    oscillatorPotentiometerArrayPage.SetItem(1, &oscLevelPotentiometerArrayPageItem);
    
    // Add octave to oscillator tab
    oscOctavePotentiometerArrayPageItem.SetScaledRange(0, 3);
    oscOctavePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscOctavePotentiometerArrayPageItem.SetTitle("OCTAVE");
    oscillatorPotentiometerArrayPage.SetItem(2, &oscOctavePotentiometerArrayPageItem);

    // Add semitone to oscillator tab
    oscSemitonePotentiometerArrayPageItem.SetScaledRange(-12, 12);
    oscSemitonePotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscSemitonePotentiometerArrayPageItem.SetTitle("SEMITONE");
    oscillatorPotentiometerArrayPage.SetItem(3, &oscSemitonePotentiometerArrayPageItem);

    // Add pulse width to oscillator tab
    oscPulseWidthPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscPulseWidthPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscPulseWidthPotentiometerArrayPageItem.SetTitle("PW");
    oscillatorPotentiometerArrayPage.SetItem(4, &oscPulseWidthPotentiometerArrayPageItem);

    // Add pulse width modulation to oscillator tab
    oscPulseWidthModulationPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscPulseWidthModulationPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscPulseWidthModulationPotentiometerArrayPageItem.SetTitle("PWM");
    oscillatorPotentiometerArrayPage.SetItem(5, &oscPulseWidthModulationPotentiometerArrayPageItem);

    // Add wave sync to oscillator tab
    oscSyncPotentiometerArrayPageItem.AddOption("OFF", 0);
    oscSyncPotentiometerArrayPageItem.AddOption("SOFT", 1);
    oscSyncPotentiometerArrayPageItem.AddOption("HARD", 2);
    oscSyncPotentiometerArrayPageItem.SetTitle("SYNC");
    oscillatorPotentiometerArrayPage.SetItem(6, &oscSyncPotentiometerArrayPageItem);

    // Add LFO depth to oscillator tab
    oscLfoDepthPotentiometerArrayPageItem.SetScaledRange(0, 255);
    oscLfoDepthPotentiometerArrayPageItem.SetOutputValue(0.5f);
    oscLfoDepthPotentiometerArrayPageItem.SetTitle("LFO MOD");
    oscillatorPotentiometerArrayPage.SetItem(7, &oscLfoDepthPotentiometerArrayPageItem);

    // Add envelope tab to tab page
    envelopeTabPageItem.Init("ENV", &envelopePotentiometerArrayPage);
    tabPage.AddItem(&envelopeTabPageItem);

    // Add attack to envelope tab
    envAttackPotentiometerArrayPageItem.SetScaledRange(0, 255);
    envAttackPotentiometerArrayPageItem.SetOutputValue(0.5f);
    envAttackPotentiometerArrayPageItem.SetTitle("ATTACK");
    envelopePotentiometerArrayPage.SetItem(3, &envAttackPotentiometerArrayPageItem);

    // Add decay to envelope tab
    envDecayPotentiometerArrayPageItem.SetScaledRange(0, 255);
    envDecayPotentiometerArrayPageItem.SetOutputValue(0.5f);
    envDecayPotentiometerArrayPageItem.SetTitle("DECAY");
    envelopePotentiometerArrayPage.SetItem(1, &envDecayPotentiometerArrayPageItem);

    // Add sustain to envelope tab
    envSustainPotentiometerArrayPageItem.SetScaledRange(0, 255);
    envSustainPotentiometerArrayPageItem.SetOutputValue(0.5f);
    envSustainPotentiometerArrayPageItem.SetTitle("SUSTAIN");
    envelopePotentiometerArrayPage.SetItem(2, &envSustainPotentiometerArrayPageItem);

    // Add release to envelope tab
    envReleasePotentiometerArrayPageItem.SetScaledRange(0, 255); 
    envReleasePotentiometerArrayPageItem.SetOutputValue(0.5f);
    envReleasePotentiometerArrayPageItem.SetTitle("RELEASE");
    envelopePotentiometerArrayPage.SetItem(0, &envReleasePotentiometerArrayPageItem);
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

void UserInterface::SetPotentiometerValues(float *values)
{
    tabPage.SetPotentiometerValues(values);
}

void UserInterface::Paint()
{
    tabPage.Paint();
    tftDisplay.Update();
}
