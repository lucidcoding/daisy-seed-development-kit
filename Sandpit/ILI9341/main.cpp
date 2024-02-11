#include "daisy_seed.h"
#include "daisysp.h"
#include "../../Packages/Daisy_ILI9394/ili9341_ui_driver.cpp"
#include "../../Hardware/PotentiometerArray/Drivers/PotentiometerArray.h"

using namespace daisy;
using namespace daisysp;
using namespace developmentKit::hardware::potentiometerArray::drivers;

DaisySeed hw;
UiDriver driver;
Encoder encoder;
Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;
PotentiometerArray potentiometerArray;
uint8_t displayValues[16];
float actualValuesSet1[16];
float actualValuesSet2[16];

void ProcessControls()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        float division = 1.0f / 256.0f;
        float hysteresisBand = division / 4.0f;
        float rawValue = potentiometerArray.analogControl[i].GetRawFloat();
        uint8_t newDisplayValue = rawValue * 256;

        // if new value is only one more and raw value is towrds the lower end of the new value

        if ((newDisplayValue == displayValues[i] + 1) && rawValue > (division * (displayValues[i] + 1)) + hysteresisBand)
        {
            displayValues[i] = newDisplayValue;
        }
        else if (newDisplayValue > displayValues[i] + 1)
        {
            displayValues[i] = newDisplayValue;
        }
        else if ((newDisplayValue == displayValues[i] - 1) && rawValue < (division * displayValues[i]) - hysteresisBand)
        {
            displayValues[i] = newDisplayValue;
        }
        else if (newDisplayValue < displayValues[i] - 1)
        {
            displayValues[i] = newDisplayValue;
        }
    }
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    potentiometerArray.Process();
    ProcessControls();
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        if (metro.Process())
        {
            gate = !gate;
        }

        adsrOut = adsr.Process(gate);
        oscillator.SetFreq(220);
        oscillator.SetAmp(adsrOut);
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
    potentiometerArray.seed = &hw;
    potentiometerArray.Init();
}

void UpdateDisplay()
{
    driver.Fill(COLOR_BLACK);

    int16_t menuWidth = 80;
    int16_t cellWidth = 60;
    int16_t cellHeight = 60;
    driver.DrawRect(Rectangle(0, 0, 319, 239), COLOR_WHITE);
    driver.DrawRect(Rectangle(0, 0, menuWidth, 239), COLOR_WHITE);

    for (uint8_t i = 0; i < 16; i++)
    {
        int16_t x = ((i % 4) * cellWidth) + menuWidth;
        int16_t y = ((i / 4) * cellWidth);
        char buffer[50];
        sprintf(buffer, "%d", displayValues[i]);
        driver.DrawRect(Rectangle(x, y, cellWidth, cellHeight), COLOR_WHITE);
        driver.WriteStringAligned(buffer, Font_11x18, Rectangle(x, y, cellWidth, cellHeight - 11), Alignment::centered, COLOR_WHITE);
        driver.WriteStringAligned("VALUE", Font_6x8, Rectangle(x, y + cellHeight - 25, cellWidth, 25), Alignment::centered, COLOR_WHITE);
    }

    driver.Update();
}

void InitControls()
{
    potentiometerArray.Process();

    for (uint8_t i = 0; i < 16; i++)
    {
        displayValues[i] = potentiometerArray.analogControl[i].GetRawFloat() * 256;
    }
}

void InitValueSets()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        actualValuesSet1[i] = 0.1;
        actualValuesSet2[i] = 0.7;
    }
}

int main(void)
{
    hw.Init(true);
    driver.Init();
    hw.StartLog(false);
    float sampleRate = hw.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitMetro(sampleRate);
    InitPotentiometerArray();
    InitControls();
    InitValueSets();
    hw.adc.Start();
    hw.StartAudio(AudioCallback);

    uint32_t lastTicksRefresh = System::GetTick();
    uint32_t lastTicksMove = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    for (;;)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;
            UpdateDisplay();
        }
    }
}