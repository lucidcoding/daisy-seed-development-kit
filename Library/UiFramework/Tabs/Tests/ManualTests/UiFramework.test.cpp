#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "../../Display.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../Presenters/PotentiometerArrayPage.h"
#include "../../View/ListPageSsd1306I2cView.h"
#include "../../View/ListPageIli9341View.h"
#include "../../View/PotentiometerArrayPageIli9341View.h"
#include "../../View/TabPageIli9341View.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"
#include "../../Presenters/TabPageItem.h"
#include "../../Presenters/TabPage.h"
#include "../../../../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"
#include "../../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9
#define PIN_ENC_A 30
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 29

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework::tabs;
using namespace developmentKit::library::uiFramework::tabs::utilities;
using namespace developmentKit::library::uiFramework::tabs::view;
using namespace developmentKit::hardware::potentiometerArray::drivers;

static DaisySeed hardware;
Encoder encoder;
static Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;
static OledDisplay<SSD130xI2c128x64Driver> oledDisplay;
static UiDriver tftDisplay;
PotentiometerArray potentiometerArray;
Display display;

UiParameter
    levelParameter,
    noteParameter,
    attackParameter,
    decayParameter,
    sustainParameter,
    releaseParameter;


PotentiometerArrayPageIli9341View potentiometerArrayPageIli9341View(&tftDisplay, 12, 22, 296, 210);
PotentiometerArrayPage potentiometerArrayPage1(&potentiometerArrayPageIli9341View);
PotentiometerArrayPage potentiometerArrayPage2(&potentiometerArrayPageIli9341View);
// OptionsSettingsPageItem *waveformSettingsPageItem;

TabPageIli9341View tabPageView(&tftDisplay);
TabPage tabPage1(&tabPageView);

void UpdateDisplay()
{
    display.Paint();
    // oledDisplay.Update();
    // tftDisplay.Update();
}

void ProcessEncoder()
{
    encoder.Debounce();
    int encoderIncrement = encoder.Increment();

    if (encoderIncrement == 1)
    {
        display.Increment();
        // UpdateDisplay();
    }

    if (encoderIncrement == -1)
    {
        display.Decrement();
        // UpdateDisplay();
    }

    if (encoder.RisingEdge())
    {
        display.Select();
        // UpdateDisplay();
    }
}

void ProcessPotentiometerArray()
{
    potentiometerArray.Process();
    float values[16];
    // float *values = potentiometerArray.GetValues();

    for (uint8_t i = 0; i < 16; i++)
    {
        values[i] = potentiometerArray.analogControl[i].GetRawFloat();
    }

    display.SetPotentiometerValues(values);
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    ProcessEncoder();
    ProcessPotentiometerArray();
    /*float level = levelParameter.Process();
    float frequency = mtof(noteParameter.Process());
    float attackTime = attackParameter.Process();
    float decayTime = decayParameter.Process();
    float sustainLevel = sustainParameter.Process();
    float releaseTime = releaseParameter.Process();
    unsigned int waveform = waveformSettingsPageItem->GetValue();
    adsr.SetTime(ADSR_SEG_ATTACK, attackTime);
    adsr.SetTime(ADSR_SEG_DECAY, decayTime);
    adsr.SetSustainLevel(sustainLevel);
    adsr.SetTime(ADSR_SEG_RELEASE, releaseTime);
    oscillator.SetWaveform(waveform);*/
    float frequency = 220;
    float level = 0.5;
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        if (metro.Process())
        {
            gate = !gate;
        }

        adsrOut = adsr.Process(gate);
        oscillator.SetFreq(frequency);
        oscillator.SetAmp(adsrOut * level);
        oscillatorOut = oscillator.Process();

        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;
    }
}

void InitOscillator(float sampleRate)
{
    oscillator.Init(sampleRate);
    oscillator.SetWaveform(Oscillator::WAVE_SQUARE);
    oscillator.SetAmp(0.125f);
    oscillator.SetFreq(mtof(64));
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, 0.0f);
    adsr.SetTime(ADSR_SEG_DECAY, 0.125f);
    adsr.SetTime(ADSR_SEG_RELEASE, 0.125f);
    adsr.SetSustainLevel(.01);
}

void InitMetro(float sampleRate)
{
    metro.Init(1.0f, sampleRate);
}

void InitPotentiometerArray()
{
    potentiometerArray.seed = &hardware;
    potentiometerArray.Init();
}

void InitTftDisplay()
{
    tftDisplay.Init();
}

void InitEncoder(float sampleRate)
{
    encoder.Init(
        hardware.GetPin(PIN_ENC_A),
        hardware.GetPin(PIN_ENC_B),
        hardware.GetPin(PIN_ENC_CLICK),
        sampleRate);
}

void InitDisplay()
{
    float initialValues[16] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
    // Setup view and home page.
    ListPageIli9341View listPageView(&tftDisplay, 0, 13, 320, 227);
    ListPage listPage1(&listPageView);
    ListPage listPage2(&listPageView);
    ListPage listPage3(&listPageView);

    potentiometerArrayPage1.SetOutputValues(initialValues);
    potentiometerArrayPage1.SetKnobMode(PotentiometerArrayPageItem::CATCH);
    potentiometerArrayPage2.SetOutputValues(initialValues);
    potentiometerArrayPage2.SetKnobMode(PotentiometerArrayPageItem::CATCH);

    // Other pages
    char title[25];
    for (uint8_t i = 0; i < 25; i++)
    {
        sprintf(title, "Page 1, %d...", i);
        listPage1.AddItem(new NavigationPageItem(title, new ListPage(&listPageView), &display));
        sprintf(title, "Page 2, %d...", i);
        listPage2.AddItem(new NavigationPageItem(title, new ListPage(&listPageView), &display));
        sprintf(title, "Page 3, %d...", i);
        listPage3.AddItem(new NavigationPageItem(title, new ListPage(&listPageView), &display));
    }

    tabPage1.AddItem(new TabPageItem("PAGE 0", &potentiometerArrayPage1));
    tabPage1.AddItem(new TabPageItem("PAGE 1", &potentiometerArrayPage2));
    tabPage1.AddItem(new TabPageItem("PAGE 2", &listPage1));
    tabPage1.AddItem(new TabPageItem("PAGE 3", &listPage2));
    tabPage1.AddItem(new TabPageItem("PAGE 4", &listPage3));
    tabPage1.AddItem(new TabPageItem("PAGE 5", new ListPage(&listPageView)));
    tabPage1.AddItem(new TabPageItem("PAGE 6", new ListPage(&listPageView)));
    tabPage1.AddItem(new TabPageItem("PAGE 7", new ListPage(&listPageView)));
    tabPage1.AddItem(new TabPageItem("PAGE 8", new ListPage(&listPageView)));
    tabPage1.AddItem(new TabPageItem("PAGE 9", new ListPage(&listPageView)));

    // Set display home page and current page.
    display.SetHomePage(&tabPage1);
    display.SetCurrentPage(&tabPage1);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    hardware.StartLog(false);
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitPotentiometerArray();
    InitMetro(sampleRate);
    InitTftDisplay();
    InitEncoder(sampleRate);
    InitDisplay();
    hardware.adc.Start();
    hardware.StartAudio(AudioCallback);

    // UpdateDisplay();

    uint32_t lastTicksRefresh = System::GetTick();
    uint32_t lastTicksRefreshDisplay = System::GetTick();
    uint32_t lastTicksRShowValues = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    while (1)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;
            UpdateDisplay();
            tftDisplay.Update();
        }

        if (currentTicks - lastTicksRShowValues > (1000000 * ticksPerUs))
        {
            lastTicksRShowValues = currentTicks;
            hardware.PrintLine("P0: %3.5f\tP1: %3.5f\tP2: %3.5f\tP3: %3.5f\t",
                               potentiometerArray.analogControl[0].GetRawFloat(),
                               potentiometerArray.analogControl[1].GetRawFloat(),
                               potentiometerArray.analogControl[2].GetRawFloat(),
                               potentiometerArray.analogControl[3].GetRawFloat());
        }
    }
}