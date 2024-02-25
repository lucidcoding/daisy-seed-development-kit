#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "ListPageView.h"
#include "../Presenters/ListPage.h"
#include "../Presenters/SettingsListPageItem.h"
#include "../ViewAdapters/ViewAdapter.h"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::presenters;

    void ListPageView::Init(ViewAdapter *prmViewAdapter, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight)
    {
        viewAdapter = prmViewAdapter;
        x = prmX;
        y = prmY;
        width = prmWidth;
        height = prmHeight;
    }

    void ListPageView::Paint(Page *page)
    {
        ListPage *listPage = static_cast<ListPage *>(page);
        const uint8_t rowHeight = 12;
        const uint8_t maxRows = height / rowHeight;
        const uint16_t valueWidth = width / 4;
        const FontDef fontDef = Font_7x10;
        viewAdapter->Fill(ViewAdapter::COLOR_BLACK);
        char title[25];
        uint8_t rowsToShow = listPage->ItemsCount() <= maxRows ? listPage->ItemsCount() : maxRows;
        uint8_t startRow = listPage->GetCurrentIndex() < maxRows ? 0 : listPage->GetCurrentIndex() - maxRows +1;

        //for (unsigned int i = 0; i < listPage->ItemsCount(); i++)
        for (uint8_t i = 0; i < rowsToShow; i++)
        {
            uint8_t currentIndex = startRow + i;
            ListPageItem *pageItem = listPage->GetItem(currentIndex);
            bool hasFocus = listPage->GetCurrentIndex() == currentIndex;
            strcpy(title, pageItem->GetTitle().c_str());
            int startPosition = y + (rowHeight * i);

            if (pageItem->GetType() == ListPageItem::ListPageItemType::NUMERIC_SETTINGS_PAGE_ITEM || pageItem->GetType() == ListPageItem::ListPageItemType::OPTIONS_SETTINGS_PAGE_ITEM)
            {
                SettingsListPageItem *settingsListPageItem = static_cast<SettingsListPageItem *>(pageItem);

                if (hasFocus && !listPage->GetItemSelected())
                {
                    viewAdapter->FillRect(x, startPosition, width, rowHeight - 1, ViewAdapter::COLOR_WHITE);
                }

                if (hasFocus && listPage->GetItemSelected())
                {
                    viewAdapter->FillRect(x + width - valueWidth, startPosition, valueWidth, rowHeight - 1, ViewAdapter::COLOR_WHITE);
                }

                viewAdapter->WriteString(title, x + 1, startPosition + 2, fontDef, (!hasFocus || listPage->GetItemSelected()) ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_BLACK);
                viewAdapter->WriteString(settingsListPageItem->GetValueAsString().c_str(), x + width - valueWidth, startPosition + 2, fontDef, !hasFocus ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_BLACK);
            }
            else
            {
                if (hasFocus)
                {
                    viewAdapter->FillRect(x, startPosition, width, rowHeight - 1, ViewAdapter::COLOR_WHITE);
                }

                viewAdapter->WriteString(title, x + 1, startPosition + 2, fontDef, !hasFocus ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_BLACK);
            }
        }
    }
}