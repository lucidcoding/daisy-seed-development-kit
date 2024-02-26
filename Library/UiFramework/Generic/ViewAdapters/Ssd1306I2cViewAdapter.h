#pragma once
#ifndef SSD1306_I2C_VIEW_ADAPTER_H
#define SSD1306_I2C_VIEW_ADAPTER_H

#include "ViewAdapter.h"
#include "dev/oled_ssd130x.h"

namespace developmentKit::library::uiFramework::tree::viewAdapters
{
    using namespace daisy;

    class Ssd1306I2cViewAdapter : public ViewAdapter
    {
    public:
        Ssd1306I2cViewAdapter() {}
        ~Ssd1306I2cViewAdapter() {}
        void Init(OledDisplay<SSD130xI2c128x64Driver> *prmDisplay);
        virtual void WriteString(const char *str, uint16_t x, uint16_t y, FontDef font, Color color);
        virtual void WriteStringAligned(const char *str, uint16_t x, uint16_t y, uint16_t width, uint16_t height, Alignment alignment, FontDef font, Color color);
        virtual void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
        virtual void FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
        virtual void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color);
        virtual void Fill(Color color);

    private:
        OledDisplay<SSD130xI2c128x64Driver> *display;
    };
}

#endif