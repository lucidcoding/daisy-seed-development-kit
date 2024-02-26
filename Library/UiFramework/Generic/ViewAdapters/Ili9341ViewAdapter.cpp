#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "Ili9341ViewAdapter.h"
#include "ViewAdapter.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tree::viewAdapters
{
    using namespace std;
    using namespace daisy;

    void Ili9341ViewAdapter::Init(UiDriver *prmDisplay)
    {
        display = prmDisplay;
    }

    void Ili9341ViewAdapter::WriteString(const char *str, uint16_t x, uint16_t y, FontDef font, Color color)
    {
        display->WriteString(str, x, y, font, color);
    }

    void Ili9341ViewAdapter::WriteStringAligned(const char *str, uint16_t x, uint16_t y, uint16_t width, uint16_t height, Alignment alignment, FontDef font, Color color)
    {
        display->WriteStringAligned(str, font, Rectangle(x, y, width, height - 11), alignment, color);
    }

    void Ili9341ViewAdapter::DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
    {
        display->DrawRect(x, y, width, height, color);
    }

    void Ili9341ViewAdapter::FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
    {
        display->FillRect(Rectangle(x, y, width, height), color);
    }

    void Ili9341ViewAdapter::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color)
    {
        display->DrawLine(x1,y1, x2, y2, color);
    }

    void Ili9341ViewAdapter::Fill(Color color)
    {
        display->Fill(color);
    }
}