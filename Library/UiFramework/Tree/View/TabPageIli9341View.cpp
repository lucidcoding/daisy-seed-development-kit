#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "TabPageIli9341View.h"
#include "../Presenters/TabPage.h"
#include "../Presenters/TabPageItem.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::tree::presenters;

    TabPageIli9341View::TabPageIli9341View(UiDriver *prmDisplayHardware)
    {
        displayHardware = prmDisplayHardware;
    }

    void TabPageIli9341View::Paint(Page *page)
    {
        
        displayHardware->Fill(COLOR_BLACK);
        displayHardware->WriteString("TEST 3", 0, 14, Font_16x26, COLOR_WHITE);

        /*TabPage *tabPage = static_cast<TabPage *>(page);
        const uint8_t x = 0;
        const uint8_t y = 0;
        const uint8_t width = 320;
        const uint8_t height = 240;
        const uint8_t rowHeight = 12;
        const uint8_t maxRows = height / rowHeight;
        const uint16_t valueWidth = width / 4;
        const FontDef fontDef = Font_7x10;
        displayHardware->Fill(COLOR_BLACK);
        char title[25];
        TabPageItem *tabPageItem = tabPage->GetItem(0);
        strcpy(title, tabPageItem->GetTitle().c_str());
        
        displayHardware->WriteString(title, x + 1, 14, fontDef, COLOR_WHITE);*/

        //displayHardware->Update();
    }
}