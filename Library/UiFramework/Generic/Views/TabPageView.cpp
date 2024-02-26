#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "TabPageView.h"
#include "../Presenters/TabPage.h"
#include "../Presenters/TabPageItem.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::presenters;

    void TabPageView::Init(ViewAdapter *prmViewAdapter, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight)
    {
        viewAdapter = prmViewAdapter;
        x = prmX;
        y = prmY;
        width = prmWidth;
        height = prmHeight;  
    }

    void TabPageView::Paint(Page *page)
    {
        TabPage *tabPage = static_cast<TabPage *>(page);
        const uint16_t width = 320;
        const uint16_t height = 240;
        const uint16_t tabWidth = 60;
        const uint16_t tabHeight = 12;
        const uint16_t tabSlopeOffset = 5;
        const FontDef fontDef = Font_7x10;
        const uint8_t maxTabs = width / tabWidth;
        viewAdapter->Fill(ViewAdapter::COLOR_BLACK);
        char title[25];
        uint8_t selectedIndex = tabPage->GetCurrentIndex();
        viewAdapter->DrawLine(0, tabHeight, width, tabHeight, ViewAdapter::COLOR_WHITE);

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

            viewAdapter->DrawLine(xOffset, 1, xOffset, tabHeight, ViewAdapter::COLOR_WHITE);
            viewAdapter->DrawLine(xOffset + 1, 0, xOffset + tabWidth - tabSlopeOffset - 1, 0, ViewAdapter::COLOR_WHITE);
            viewAdapter->DrawLine(xOffset + tabWidth - tabSlopeOffset, 0, xOffset + tabWidth, tabHeight, ViewAdapter::COLOR_WHITE);
            viewAdapter->WriteString(title, xOffset + 5, 2, fontDef, ViewAdapter::COLOR_WHITE);

            if (currentIndex == selectedIndex)
            {
                viewAdapter->DrawLine(xOffset + 1, tabHeight, xOffset + tabWidth - 1, tabHeight, ViewAdapter::COLOR_BLACK);
                tabPageItem->GetContent()->Paint();
            }
        }

        viewAdapter->DrawLine(0, tabHeight, 0, height - 1, ViewAdapter::COLOR_WHITE);
        viewAdapter->DrawLine(0, height - 1, width - 1, height - 1, ViewAdapter::COLOR_WHITE);
        viewAdapter->DrawLine(width - 1, tabHeight - 1, width - 1, height - 1, ViewAdapter::COLOR_WHITE);
    }
}