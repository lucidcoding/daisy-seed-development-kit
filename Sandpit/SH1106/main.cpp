#include "daisy_seed.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
OledDisplay<SSD130x4WireSpi128x64Driver> display;

int main(void)
{
    hw.Init(true);
    hw.StartLog(false);

    OledDisplay<SSD130x4WireSpi128x64Driver>::Config spi_config;
    /*spi_config.driver_config.transport_config.spi_config.periph = SpiHandle::Config::Peripheral::SPI_1;
    spi_config.driver_config.transport_config.spi_config.mode = SpiHandle::Config::Mode::MASTER;
    spi_config.driver_config.transport_config.spi_config.direction = SpiHandle::Config::Direction::TWO_LINES_TX_ONLY;
    spi_config.driver_config.transport_config.spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
    spi_config.driver_config.transport_config.spi_config.baud_prescaler = SpiHandle::Config::BaudPrescaler::PS_2;
    spi_config.driver_config.transport_config.spi_config.clock_phase = SpiHandle::Config::ClockPhase::ONE_EDGE;
    spi_config.driver_config.transport_config.spi_config.nss = SpiHandle::Config::NSS::HARD_OUTPUT;
    spi_config.driver_config.transport_config.spi_config.datasize = 8;
    spi_config.driver_config.transport_config.spi_config.pin_config.sclk = {DSY_GPIOG, 11};
    spi_config.driver_config.transport_config.spi_config.pin_config.mosi = {DSY_GPIOB, 5};
    spi_config.driver_config.transport_config.spi_config.pin_config.nss = {DSY_GPIOG, 10};*/
    spi_config.driver_config.transport_config.pin_config.dc = hw.GetPin(22);
    spi_config.driver_config.transport_config.pin_config.reset = hw.GetPin(16);
    display.Init(spi_config);
    // spi_config.pin_config.miso = {DSY_GPIOX, 0}; // not used

    dsy_gpio pin_reset_;
    dsy_gpio pin_dc_;
    dsy_gpio pin_cs_;
    // Here all the drawing happening in the memory buffer, so no drawing happening at this point.
    uint8_t dc_pin = 16;
    uint8_t reset_pin = 22;

    // DC pin
    /*pin_dc_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    pin_dc_.pin = DaisySeed::GetPin(dc_pin);
    dsy_gpio_init(&pin_dc_);
    // Reset pin
    pin_reset_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    pin_reset_.pin = DaisySeed::GetPin(reset_pin);
    dsy_gpio_init(&pin_reset_);
    // CS pin
    pin_cs_.mode = DSY_GPIO_MODE_OUTPUT_PP;
    pin_cs_.pin = spi_config.driver_config.transport_config.spi_config.pin_config.nss;
    dsy_gpio_init(&pin_cs_);*/

    display.Fill(true);
    char title[25] = "ttt";
    display.SetCursor(1, 10);
    display.WriteString(title, Font_7x10, true);

    for (;;)
    {
        // Update() is required to actually flush the screen buffer to the display
        System::Delay(1000);
        display.Update();
        hw.PrintLine("tick...");
    }
}