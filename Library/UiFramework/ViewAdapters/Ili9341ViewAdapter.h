#pragma once
#ifndef ILI9341_VIEW_ADAPTER_H
#define ILI9341_VIEW_ADAPTER_H

#include "ViewAdapter.h"
#include "../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::viewAdapters
{
    class Ili9341ViewAdapter : public ViewAdapter
    {
    public:
        Ili9341ViewAdapter() {}
        ~Ili9341ViewAdapter() {}
        void Init(UiDriver *prmDisplay);
        virtual void WriteString(const char *str, uint16_t x, uint16_t y, FontDef font, Color color);
        virtual void WriteStringAligned(const char *str, uint16_t x, uint16_t y, uint16_t width, uint16_t height, Alignment alignment, FontDef font, Color color);
        virtual void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
        virtual void FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color);
        virtual void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color);
        virtual void Fill(Color color);

    private:
        UiDriver *display;
    };
}

#endif