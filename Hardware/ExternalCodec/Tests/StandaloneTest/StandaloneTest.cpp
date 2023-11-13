#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;

static DaisySeed hw;
static Oscillator osc;

void ConfigureAudio()
{
    // SAI2 -- Peripheral
    // Configure
    SaiHandle::Config sai_config;
    sai_config.periph = SaiHandle::Config::Peripheral::SAI_2;
    sai_config.sr = SaiHandle::Config::SampleRate::SAI_48KHZ;
    sai_config.bit_depth = SaiHandle::Config::BitDepth::SAI_24BIT;
    /*sai_config.a_sync = SaiHandle::Config::Sync::MASTER;
    sai_config.b_sync = SaiHandle::Config::Sync::SLAVE;*/
    /*sai_config[1].a_sync = SaiHandle::Config::Sync::SLAVE;
    sai_config[1].b_sync = SaiHandle::Config::Sync::MASTER;*/
    sai_config.a_sync = SaiHandle::Config::Sync::SLAVE;
    sai_config.b_sync = SaiHandle::Config::Sync::MASTER;
    sai_config.pin_config.fs = hw.GetPin(27);
    sai_config.pin_config.mclk = hw.GetPin(24);
    sai_config.pin_config.sck = hw.GetPin(28);
    // Data Line Directions
    sai_config.a_dir = SaiHandle::Config::Direction::RECEIVE;
    sai_config.pin_config.sa = hw.GetPin(26);
    sai_config.b_dir = SaiHandle::Config::Direction::TRANSMIT;
    sai_config.pin_config.sb = hw.GetPin(25);
    I2CHandle::Config i2c_config;
    I2CHandle i2c_handle;
    Wm8731::Config codec_cfg;
    Wm8731 codec;
    i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
    i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
    i2c_config.speed = I2CHandle::Config::Speed::I2C_400KHZ;
    i2c_config.pin_config.scl = hw.GetPin(11);
    i2c_config.pin_config.sda = hw.GetPin(12);
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
    hw.audio_handle.Init(audio_config, sai_2_handle);
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t size)
{
    float sig;
    for (size_t i = 0; i < size; i += 2)
    {
        sig = osc.Process();

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    // initialize seed hardware and oscillator daisysp module
    float sample_rate;
    hw.Configure();
    hw.Init();
    ConfigureAudio();
    hw.SetAudioBlockSize(4);
    sample_rate = hw.AudioSampleRate();
    osc.Init(sample_rate);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    // start callback
    hw.StartAudio(AudioCallback);

    while (1)
    {
    }
}