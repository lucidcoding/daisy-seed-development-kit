#include "daisy_seed.h"
#include "daisysp.h"
#include "../../Packages/Daisy_ILI9394/ili9341_ui_driver.cpp"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
UiDriver driver;
Encoder encoder;
Oscillator oscillator;
Adsr adsr;
Metro metro;
bool gate;

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
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


int main(void)
{
    hw.Init(true);
    driver.Init();
    float sampleRate = hw.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    InitMetro(sampleRate);
    hw.StartAudio(AudioCallback);

    // Here all the drawing happening in the memory buffer, so no drawing happening at this point.
    driver.Fill(COLOR_BLACK);
    driver.FillRect(Rectangle(1, 100, 50, 50), COLOR_RED);
    driver.DrawRect(Rectangle(0, 0, 319, 239), COLOR_WHITE);
    // driver.DrawCircle(150, 150, 50, COLOR_GREEN);
    driver.Update();

    uint32_t lastTicksRefresh = System::GetTick();
    uint32_t lastTicksMove = System::GetTick();
    const uint32_t ticksPerUs = System::GetTickFreq() / 1000000;

    int16_t radius = 10;

    for (;;)
    {
        uint32_t currentTicks = System::GetTick();

        if (currentTicks - lastTicksRefresh > (100000 * ticksPerUs))
        {
            lastTicksRefresh = currentTicks;
            driver.Update();
        }

        if (currentTicks - lastTicksMove > (20000 * ticksPerUs))
        {
            lastTicksMove = currentTicks;

            driver.DrawCircle(150, 150, radius, COLOR_BLACK);
            radius = (radius + 2) % 100;
            driver.DrawCircle(150, 150, radius, COLOR_GREEN);

        }
    }
}