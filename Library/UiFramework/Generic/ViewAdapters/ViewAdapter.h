#pragma once
#ifndef VIEW_ADAPTER_H
#define VIEW_ADAPTER_H

#include <stdint.h>
#include "daisy_seed.h"
#include "util/oled_fonts.h"

namespace developmentKit::library::uiFramework::tree::viewAdapters
{
    using namespace daisy;

    class ViewAdapter
    {
    public:
        enum Color
        {
            COLOR_BLACK = 0,
            COLOR_WHITE,
            COLOR_BLUE,
            COLOR_DARK_BLUE,
            COLOR_CYAN,
            COLOR_YELLOW,
            COLOR_DARK_YELLOW,
            COLOR_ORANGE,
            COLOR_RED,
            COLOR_DARK_RED,
            COLOR_GREEN,
            COLOR_DARK_GREEN,
            COLOR_LIGHT_GREEN,
            COLOR_GRAY,
            COLOR_DARK_GRAY,
            COLOR_LIGHT_GRAY,
            COLOR_MEDIUM_GRAY,
            COLOR_ABL_BG,
            COLOR_ABL_LINE,
            COLOR_ABL_D_LINE,
            COLOR_ABL_L_GRAY,
            COLOR_ABL_M_GRAY,
            NUMBER_OF_TFT_COLORS
        };
        virtual void WriteString(const char *str, uint16_t x, uint16_t y, FontDef font, Color color) = 0;
        virtual void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) = 0;
        virtual void FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) = 0;
        virtual void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color) = 0;
        virtual void Fill(Color color) = 0;

    private:
    };
}

#endif