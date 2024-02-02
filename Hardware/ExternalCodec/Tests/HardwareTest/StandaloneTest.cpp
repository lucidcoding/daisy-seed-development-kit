#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed hardware;
static Oscillator mainOsc;
Adsr adsr;
bool gate;

void ConfigureAudio()
{
    SaiHandle::Config sai_config;
    sai_config.periph = SaiHandle::Config::Peripheral::SAI_2;
    sai_config.sr = SaiHandle::Config::SampleRate::SAI_48KHZ;
    sai_config.bit_depth = SaiHandle::Config::BitDepth::SAI_24BIT;
    sai_config.a_sync = SaiHandle::Config::Sync::SLAVE;
    sai_config.b_sync = SaiHandle::Config::Sync::MASTER;
    sai_config.pin_config.fs = hardware.GetPin(27);
    sai_config.pin_config.mclk = hardware.GetPin(24);
    sai_config.pin_config.sck = hardware.GetPin(28);
    sai_config.a_dir = SaiHandle::Config::Direction::RECEIVE;
    sai_config.pin_config.sa = hardware.GetPin(26);
    sai_config.b_dir = SaiHandle::Config::Direction::TRANSMIT;
    sai_config.pin_config.sb = hardware.GetPin(25);
    I2CHandle::Config i2c_config;
    I2CHandle i2c_handle;
    Wm8731::Config codec_cfg;
    Wm8731 codec;
    i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
    i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    i2c_config.speed = I2CHandle::Config::Speed::I2C_400KHZ;
    i2c_config.pin_config.scl = hardware.GetPin(11); //{DSY_GPIOB, 8};
    i2c_config.pin_config.sda = hardware.GetPin(12); //{DSY_GPIOB, 9};
    i2c_handle.Init(i2c_config);
    codec_cfg.Defaults();
    codec.Init(codec_cfg, i2c_handle);

    // Then Initialize
    SaiHandle sai_2_handle;
    sai_2_handle.Init(sai_config);

    // Audio
    AudioHandle::Config audio_config;
    audio_config.blocksize = 48;
    audio_config.samplerate = SaiHandle::Config::SampleRate::SAI_48KHZ;
    audio_config.postgain = .5f;
    hardware.audio_handle.Init(audio_config, sai_2_handle);
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float oscillatorOut, adsrOut;

    for (size_t i = 0; i < size; i += 2)
    {
        adsrOut = adsr.Process(gate);
        mainOsc.SetAmp(adsrOut / 2);
        oscillatorOut = mainOsc.Process();
        out[i] = oscillatorOut;
        out[i + 1] = oscillatorOut;
    }
}

void InitOscillator(float sampleRate)
{
    mainOsc.Init(sampleRate);
    mainOsc.SetWaveform(Oscillator::WAVE_POLYBLEP_TRI);
    mainOsc.SetAmp(1);
}

void InitAdsr(float sampleRate)
{
    adsr.Init(sampleRate);
    adsr.SetTime(ADSR_SEG_ATTACK, .01);
    adsr.SetTime(ADSR_SEG_DECAY, .1);
    adsr.SetTime(ADSR_SEG_RELEASE, .1);
    adsr.SetSustainLevel(.1);
}

int main(void)
{
    hardware.Configure();
    hardware.Init();
    ConfigureAudio();
    hardware.SetAudioBlockSize(4); 
    float sampleRate = hardware.AudioSampleRate();
    InitOscillator(sampleRate);
    InitAdsr(sampleRate);
    hardware.StartAudio(AudioCallback);

    while (1)
    {
        System::Delay(1000);
        gate = !gate;
    }
}