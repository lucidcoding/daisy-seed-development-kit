#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "ListPageIli9341View.h"
#include "../Presenters/ListPage.h"
#include "../Presenters/SettingsPageItem.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::presenters;

    ListPageIli9341View::ListPageIli9341View(UiDriver *prmDisplayHardware)
    {
        displayHardware = prmDisplayHardware;
        x = 0;
        y = 0;
        width = 320;
        height = 240;
    }

    ListPageIli9341View::ListPageIli9341View(UiDriver *prmDisplayHardware, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight)
    {
        displayHardware = prmDisplayHardware;
        x = prmX;
        y = prmY;
        width = prmWidth;
        height = prmHeight;
    }

    void ListPageIli9341View::Paint(Page *page)
    {
        ListPage *listPage = static_cast<ListPage *>(page);
        const uint8_t rowHeight = 12;
        const uint8_t maxRows = height / rowHeight;
        const uint16_t valueWidth = width / 4;
        const FontDef fontDef = Font_7x10;
        displayHardware->Fill(COLOR_BLACK);
        char title[25];
        uint8_t rowsToShow = listPage->ItemsCount() <= maxRows ? listPage->ItemsCount() : maxRows;
        uint8_t startRow = listPage->GetCurrentIndex() < maxRows ? 0 : listPage->GetCurrentIndex() - maxRows +1;

        for (uint8_t i = 0; i < rowsToShow; i++)
        {
            uint8_t currentIndex = startRow + i;
            PageItem *pageItem = listPage->GetItem(currentIndex);
            bool hasFocus = listPage->GetCurrentIndex() == currentIndex;
            strcpy(title, pageItem->GetTitle().c_str());
            int startPosition = y + (rowHeight * i);

            if (pageItem->GetType() == PageItem::PageItemType::NUMERIC_SETTINGS_PAGE_ITEM || pageItem->GetType() == PageItem::PageItemType::OPTIONS_SETTINGS_PAGE_ITEM)
            {
                SettingsPageItem *settingsPageItem = static_cast<SettingsPageItem *>(pageItem);

                if (hasFocus && !listPage->GetItemSelected())
                {
                    displayHardware->FillRect(Rectangle(x, startPosition, width, rowHeight - 1), COLOR_WHITE);
                }

                if (hasFocus && listPage->GetItemSelected())
                {
                    displayHardware->FillRect(Rectangle(x + width - valueWidth, startPosition, valueWidth, rowHeight - 1), COLOR_WHITE);
                }

                displayHardware->WriteString(title, x + 1, startPosition + 2, fontDef, !hasFocus || listPage->GetItemSelected() ? COLOR_WHITE : COLOR_BLACK);
                displayHardware->WriteString(settingsPageItem->GetValueAsString().c_str(), x + width - valueWidth, startPosition +2, fontDef, !hasFocus ? COLOR_WHITE : COLOR_BLACK);
            }
            else
            {
                if (hasFocus)
                {
                    displayHardware->FillRect(Rectangle(x, startPosition, width, rowHeight - 1), COLOR_WHITE);
                }

                displayHardware->WriteString(title, x + 1, startPosition + 2, fontDef, !hasFocus ? COLOR_WHITE : COLOR_BLACK);
            }
        }

        //displayHardware->Update();
    }
}