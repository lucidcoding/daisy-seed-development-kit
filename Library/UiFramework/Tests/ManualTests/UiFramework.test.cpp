#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "../../Display.h"
#include "../../Utilities/UiParameter.h"
#include "../../Presenters/ListPage.h"
#include "../../View/ListPageView.h"
#include "../../Presenters/NavigationPageItem.h"
#include "../../Presenters/OptionsSettingsPageItem.h"

#define PIN_I2C_SCL 8
#define PIN_I2C_SDA 9
#define PIN_ENC_A 30
#define PIN_ENC_CLICK 0
#define PIN_ENC_B 29

using namespace daisysp;
using namespace daisy;
using namespace developmentKit::library::uiFramework;
using namespace developmentKit::library::uiFramework::utilities;
using namespace developmentKit::library::uiFramework::view;

static DaisySeed hardware;
Encoder encoder;
Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;
OledDisplay<SSD130xI2c128x64Driver> oledDisplay;
Display display;

UiParameter
    levelParameter,
    noteParameter,
    attackParameter,
    decayParameter,
    sustainParameter,
    releaseParameter;

OptionsSettingsPageItem *waveformSettingsPageItem;

void UpdateDisplay()
{
    display.Paint();
}

void ProcessEncoder()
{
    encoder.Debounce();
    int encoderIncrement = encoder.Increment();

    if (encoderIncrement == 1)
    {
        display.Increment();
        UpdateDisplay();
    }

    if (encoderIncrement == -1)
    {
        display.Decrement();
        UpdateDisplay();
    }

    if (encoder.RisingEdge())
    {
        display.Select();
        UpdateDisplay();
    }
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    ProcessEncoder();
    float level = levelParameter.Process();
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
    oscillator.SetWaveform(waveform);
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
    oscillator.SetWaveform(Oscillator::WAVE_SAW);
    oscillator.SetAmp(0.25f);
    oscillator.SetFreq(220);
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .05);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.01);
}

void InitMetro(float sampleRate)
{
    metro.Init(1.0f, sampleRate);
}

void InitOledDisplay()
{
    OledDisplay<SSD130xI2c128x64Driver>::Config disp_cfg;
    disp_cfg.driver_config.transport_config.i2c_address = 0x3C;
    disp_cfg.driver_config.transport_config.i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    disp_cfg.driver_config.transport_config.i2c_config.speed = I2CHandle::Config::Speed::I2C_1MHZ;
    disp_cfg.driver_config.transport_config.i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, PIN_I2C_SCL};
    disp_cfg.driver_config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, PIN_I2C_SDA};
    oledDisplay.Init(disp_cfg);
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
    // Setup view and home page.
    ListPageView *listPageView = new ListPageView(&oledDisplay);
    ListPage *home = new ListPage(listPageView);

    // Setup oscillator page and add to home page.
    ListPage *oscillatorListPage = new ListPage(listPageView);
    oscillatorListPage->AddItem(new NavigationPageItem("Back...", home, &display));
    NumericSettingsPageItem *levelSettingsPageItem = new NumericSettingsPageItem("Level", oscillatorListPage, 0, 127, 16);
    oscillatorListPage->AddItem(levelSettingsPageItem);
    NumericSettingsPageItem *noteSettingsPageItem = new NumericSettingsPageItem("Note", oscillatorListPage, 0, 127, 64);
    oscillatorListPage->AddItem(noteSettingsPageItem);
    waveformSettingsPageItem = new OptionsSettingsPageItem("Waveform", oscillatorListPage);
    waveformSettingsPageItem->AddOption("Sin", Oscillator::WAVE_SIN);
    waveformSettingsPageItem->AddOption("Tri", Oscillator::WAVE_TRI);
    waveformSettingsPageItem->AddOption("Saw", Oscillator::WAVE_SAW);
    waveformSettingsPageItem->AddOption("Squ", Oscillator::WAVE_SQUARE);
    oscillatorListPage->AddItem(waveformSettingsPageItem);
    home->AddItem(new NavigationPageItem("Oscillator...", oscillatorListPage, &display));

    // Setup envelope page and add to home page.
    ListPage *adsrListPage = new ListPage(listPageView);
    adsrListPage->AddItem(new NavigationPageItem("Back...", home, &display));
    NumericSettingsPageItem *attackSettingsPageItem = new NumericSettingsPageItem("Attack", adsrListPage, 0, 127, 0);
    adsrListPage->AddItem(attackSettingsPageItem);
    NumericSettingsPageItem *decaySettingsPageItem = new NumericSettingsPageItem("Delay", adsrListPage, 0, 127, 12);
    adsrListPage->AddItem(decaySettingsPageItem);
    NumericSettingsPageItem *sustainSettingsPageItem = new NumericSettingsPageItem("Sustain", adsrListPage, 0, 127, 16);
    adsrListPage->AddItem(sustainSettingsPageItem);
    NumericSettingsPageItem *releaseSettingsPageItem = new NumericSettingsPageItem("Release", adsrListPage, 0, 127, 16);
    adsrListPage->AddItem(releaseSettingsPageItem);
    home->AddItem(new NavigationPageItem("Envelope...", adsrListPage, &display));

    // Set display home page and current page.
    display.SetHomePage(home);
    display.SetCurrentPage(home);

    // Tie parameters to values from settings page items.
    levelParameter.Init(levelSettingsPageItem, 0.0f, 1.0f, UiParameter::LINEAR);
    noteParameter.Init(noteSettingsPageItem, 0, 128, UiParameter::LINEAR);
    attackParameter.Init(attackSettingsPageItem, 0.0f, 1.0f, UiParameter::LINEAR);
    decayParameter.Init(decaySettingsPageItem, 0.0f, 1.0f, UiParameter::LINEAR);
    sustainParameter.Init(sustainSettingsPageItem, 0.0f, 1.0f, UiParameter::LINEAR);
    releaseParameter.Init(releaseSettingsPageItem, 0.0f, 1.0f, UiParameter::LINEAR);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitMetro(sampleRate);
    InitOledDisplay();
    InitEncoder(sampleRate);
    InitDisplay();
    hardware.StartAudio(AudioCallback);
    UpdateDisplay();

    while (1)
    {}
}