#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "ListPageSsd1306I2cView.h"
#include "../Presenters/ListPage.h"
#include "../Presenters/SettingsPageItem.h"

namespace developmentKit::library::uiFramework::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::presenters;

    ListPageSsd1306I2cView::ListPageSsd1306I2cView(OledDisplay<SSD130xI2c128x64Driver> *prmDisplay)
    {
        display = prmDisplay;
    }

    void ListPageSsd1306I2cView::Paint(Page *page)
    {
        ListPage *listPage = static_cast<ListPage *>(page);
        const int rowHeight = 12;
        display->Fill(false);
        char title[25];

        for (unsigned int i = 0; i < listPage->ItemsCount(); i++)
        {
            int offset = 0;

            if (listPage->GetCurrentIndex() > 4)
            {
                offset = rowHeight * (listPage->GetCurrentIndex() - 4);
            }

            PageItem *pageItem = listPage->GetItem(i);
            bool hasFocus = listPage->GetCurrentIndex() == i;
            strcpy(title, pageItem->GetTitle().c_str());
            int startPosition = (rowHeight * i) - offset;

            if (pageItem->GetType() == PageItem::PageItemType::NUMERIC_SETTINGS_PAGE_ITEM || pageItem->GetType() == PageItem::PageItemType::OPTIONS_SETTINGS_PAGE_ITEM)
            {
                SettingsPageItem *settingsPageItem = static_cast<SettingsPageItem *>(pageItem);

                if (hasFocus && !listPage->GetItemSelected())
                {
                    display->DrawRect(0, startPosition, 128, rowHeight + startPosition - 1, true, true);
                }

                if (hasFocus && listPage->GetItemSelected())
                {
                    display->DrawRect(99, startPosition, 128, rowHeight + startPosition - 1, true, true);
                }

                display->SetCursor(1, startPosition + 2);
                display->WriteString(title, Font_7x10, !hasFocus || listPage->GetItemSelected());
                display->SetCursor(100, startPosition + 2);
                display->WriteString(settingsPageItem->GetValueAsString().c_str(), Font_7x10, !hasFocus);
            }
            else
            {
                if (hasFocus)
                {
                    display->DrawRect(0, startPosition, 128, rowHeight + startPosition - 1, true, true);
                }

                display->SetCursor(1, startPosition + 2);
                display->WriteString(title, Font_7x10, !hasFocus);
            }
        }

        display->Update();
    }
}