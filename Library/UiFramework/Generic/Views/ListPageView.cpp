#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "ListPageView.h"
#include "../Presenters/ListPage.h"
#include "../Presenters/SettingsPageItem.h"
#include "../ViewAdapters/ViewAdapter.h"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::presenters;

    void ListPageView::Init(ViewAdapter *prmViewAdapter)
    {
        viewAdapter = prmViewAdapter;
    }

    void ListPageView::Paint(Page *page)
    {
        ListPage *listPage = static_cast<ListPage *>(page);
        const int rowHeight = 12;
        viewAdapter->Fill(ViewAdapter::COLOR_BLACK);
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
                    viewAdapter->FillRect(0, startPosition, 128, rowHeight - 1, ViewAdapter::COLOR_WHITE);
                }

                if (hasFocus && listPage->GetItemSelected())
                {
                    viewAdapter->FillRect(99, startPosition, 128, rowHeight - 1, ViewAdapter::COLOR_WHITE);
                }

                //display->SetCursor(1, startPosition + 2);
                //display->WriteString(title, Font_7x10, !hasFocus || listPage->GetItemSelected());
                viewAdapter->WriteString(title, 1, startPosition + 2, Font_7x10, (!hasFocus || listPage->GetItemSelected()) ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_BLACK);
                //display->SetCursor(100, startPosition + 2);
                //display->WriteString(settingsPageItem->GetValueAsString().c_str(), Font_7x10, !hasFocus);
                viewAdapter->WriteString(settingsPageItem->GetValueAsString().c_str(), 100, startPosition + 2, Font_7x10, !hasFocus ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_BLACK);
            }
            else
            {
                if (hasFocus)
                {
                    //display->DrawRect(0, startPosition, 128, rowHeight + startPosition - 1, true, true);
                    viewAdapter->FillRect(0, startPosition, 128, rowHeight - 1, ViewAdapter::COLOR_WHITE);
                }

                //display->SetCursor(1, startPosition + 2);
                //display->WriteString(title, Font_7x10, !hasFocus);
                viewAdapter->WriteString(title, 1, startPosition + 2, Font_7x10, !hasFocus ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_BLACK);
            }
        }
    }
}