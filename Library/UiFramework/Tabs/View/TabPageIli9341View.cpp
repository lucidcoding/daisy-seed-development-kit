#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "TabPageIli9341View.h"
#include "../Presenters/TabPage.h"
#include "../Presenters/TabPageItem.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tabs::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::tabs::presenters;

    TabPageIli9341View::TabPageIli9341View(UiDriver *prmDisplayHardware)
    {
        displayHardware = prmDisplayHardware;
    }

    void TabPageIli9341View::Paint(Page *page)
    {
        TabPage *tabPage = static_cast<TabPage *>(page);
        const uint16_t x = 0;
        const uint16_t y = 0;
        const uint16_t width = 320;
        const uint16_t height = 240;
        const uint16_t tabWidth = 60;
        const uint16_t tabHeight = 12;
        const uint16_t tabSlopeOffset = 5;
        const FontDef fontDef = Font_7x10;
        const uint8_t maxTabs = width / tabWidth;
        displayHardware->Fill(COLOR_BLACK);
        char title[25];
        uint8_t selectedIndex = tabPage->GetCurrentIndex();
        displayHardware->DrawLine(0, tabHeight, width, tabHeight, COLOR_WHITE);

        uint8_t tabsToShow;

        if (tabPage->ItemsCount() <= maxTabs)
        {
            tabsToShow = tabPage->ItemsCount();
        }
        else if (tabPage->GetCurrentIndex() == tabPage->ItemsCount() - 1)
        {
            tabsToShow = maxTabs;
        }
        else
        {
            tabsToShow = maxTabs + 1;
        }

        uint8_t startTab = tabPage->GetCurrentIndex() < maxTabs ? 0 : tabPage->GetCurrentIndex() - maxTabs + 1;

        for (uint8_t visibleTabIndex = 0; visibleTabIndex < tabsToShow; visibleTabIndex++)
        {
            uint8_t currentIndex = startTab + visibleTabIndex;
            TabPageItem *tabPageItem = tabPage->GetItem(currentIndex);
            strcpy(title, tabPageItem->GetTitle().c_str());
            uint16_t xOffset = visibleTabIndex * tabWidth;

            displayHardware->DrawLine(xOffset, 1, xOffset, tabHeight, COLOR_WHITE);
            displayHardware->DrawLine(xOffset + 1, 0, xOffset + tabWidth - tabSlopeOffset - 1, 0, COLOR_WHITE);
            displayHardware->DrawLine(xOffset + tabWidth - tabSlopeOffset, 0, xOffset + tabWidth, tabHeight, COLOR_WHITE);
            displayHardware->WriteString(title, xOffset + 5, 2, fontDef, COLOR_WHITE);

            if (currentIndex == selectedIndex)
            {
                displayHardware->DrawLine(xOffset + 1, tabHeight, xOffset + tabWidth - 1, tabHeight, COLOR_BLACK);
                tabPageItem->GetContent()->Paint();
            }
        }

        displayHardware->DrawLine(0, tabHeight, 0, height - 1, COLOR_WHITE);
        displayHardware->DrawLine(0, height - 1, width - 1, height - 1, COLOR_WHITE);
        displayHardware->DrawLine(width - 1, tabHeight - 1, width - 1, height - 1, COLOR_WHITE);
    }
}