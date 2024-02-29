#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "Ssd1306I2cViewAdapter.h"
#include "ViewAdapter.h"

namespace developmentKit::library::uiFramework::viewAdapters
{
    void Ssd1306I2cViewAdapter::Init(OledDisplay<SSD130xI2c128x64Driver> *prmDisplay)
    {
        display = prmDisplay;
    }

    void Ssd1306I2cViewAdapter::WriteString(const char *str, uint16_t x, uint16_t y, FontDef font, Color color)
    {
        display->SetCursor(x, y);
        display->WriteString(str, font, (color == ViewAdapter::COLOR_WHITE));
    }

    void Ssd1306I2cViewAdapter::WriteStringAligned(const char *str, uint16_t x, uint16_t y, uint16_t width, uint16_t height, Alignment alignment, FontDef font, Color color)
    {
    }

    void Ssd1306I2cViewAdapter::DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
    {
        display->DrawRect(x, y, x+ width, y + height, (color == ViewAdapter::COLOR_WHITE), false);
    }

    void Ssd1306I2cViewAdapter::FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
    {
        display->DrawRect(x, y, x+ width, y + height, (color == ViewAdapter::COLOR_WHITE), true);
    }

    void Ssd1306I2cViewAdapter::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color)
    {
    }

    void Ssd1306I2cViewAdapter::Fill(Color color)
    {
        display->Fill((color == ViewAdapter::COLOR_WHITE));
    }
}