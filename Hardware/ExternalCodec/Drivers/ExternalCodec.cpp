#include "ExternalCodec.h"
#include "daisysp.h"
#include "daisy_seed.h"
#include "dev/codec_ak4556.h"
#include "dev/codec_wm8731.h"

namespace developmentKit::hardware::externalCodec::drivers
{
    using namespace daisysp;
    using namespace daisy;

    void ExternalCodec::Init(DaisySeed *seed)
    {
        // Handle Seed Audio as-is and then
        SaiHandle::Config sai_config[2];

        // Internal Codec
        if (seed->CheckBoardVersion() == DaisySeed::BoardVersion::DAISY_SEED_1_1)
        {
            sai_config[0].pin_config.sa = {DSY_GPIOE, 6};
            sai_config[0].pin_config.sb = {DSY_GPIOE, 3};
            sai_config[0].a_dir = SaiHandle::Config::Direction::RECEIVE;
            sai_config[0].b_dir = SaiHandle::Config::Direction::TRANSMIT;
        }
        else
        {
            sai_config[0].pin_config.sa = {DSY_GPIOE, 6};
            sai_config[0].pin_config.sb = {DSY_GPIOE, 3};
            sai_config[0].a_dir = SaiHandle::Config::Direction::TRANSMIT;
            sai_config[0].b_dir = SaiHandle::Config::Direction::RECEIVE;
        }
        sai_config[0].periph = SaiHandle::Config::Peripheral::SAI_1;
        sai_config[0].sr = SaiHandle::Config::SampleRate::SAI_48KHZ;
        sai_config[0].bit_depth = SaiHandle::Config::BitDepth::SAI_24BIT;
        sai_config[0].a_sync = SaiHandle::Config::Sync::MASTER;
        sai_config[0].b_sync = SaiHandle::Config::Sync::SLAVE;
        sai_config[0].pin_config.fs = {DSY_GPIOE, 4};
        sai_config[0].pin_config.mclk = {DSY_GPIOE, 2};
        sai_config[0].pin_config.sck = {DSY_GPIOE, 5};

        // External Codec
        // Set up like Seed rev 5:
        // https://github.com/electro-smith/libDaisy/blob/master/src/daisy_seed.cpp#L269
        sai_config[1].periph = SaiHandle::Config::Peripheral::SAI_2;
        sai_config[1].sr = SaiHandle::Config::SampleRate::SAI_48KHZ;
        sai_config[1].bit_depth = SaiHandle::Config::BitDepth::SAI_24BIT;
        sai_config[1].a_sync = SaiHandle::Config::Sync::SLAVE;
        sai_config[1].b_sync = SaiHandle::Config::Sync::MASTER;
        sai_config[1].a_dir = SaiHandle::Config::Direction::RECEIVE;
        sai_config[1].b_dir = SaiHandle::Config::Direction::TRANSMIT;
        sai_config[1].pin_config.fs = seed->GetPin(27);
        sai_config[1].pin_config.mclk = seed->GetPin(24);
        sai_config[1].pin_config.sck = seed->GetPin(28);
        sai_config[1].pin_config.sb = seed->GetPin(25);
        sai_config[1].pin_config.sa = seed->GetPin(26);
        I2CHandle::Config i2c_config;
        i2c_config.mode = I2CHandle::Config::Mode::I2C_MASTER;
        i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
        i2c_config.speed = I2CHandle::Config::Speed::I2C_400KHZ;
        i2c_config.pin_config.scl = seed->GetPin(11);
        i2c_config.pin_config.sda = seed->GetPin(12);
        I2CHandle i2c_handle;
        i2c_handle.Init(i2c_config);
        Wm8731::Config codec_cfg;
        codec_cfg.Defaults();
        Wm8731 codec;
        codec.Init(codec_cfg, i2c_handle);

        SaiHandle sai_handle[2];
        sai_handle[0].Init(sai_config[0]);
        sai_handle[1].Init(sai_config[1]);

        // Reset Pin for AK4556
        // Built-in AK4556 was reset during Seed Init
        //dsy_gpio_pin codec_reset_pin = seed->GetPin(29);
        //Ak4556::Init(codec_reset_pin);

        // Reinit Audio for _both_ codecs...
        AudioHandle::Config cfg;
        cfg.blocksize = 48;
        cfg.samplerate = SaiHandle::Config::SampleRate::SAI_48KHZ;
        // cfg.postgain   = 0.5f;
        cfg.postgain = 1.f;
        seed->audio_handle.Init(cfg, sai_handle[0], sai_handle[1]);
    }
}
