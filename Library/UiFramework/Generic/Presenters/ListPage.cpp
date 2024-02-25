#include "ListPage.h"
#include "ListPageItem.h"
#include "Page.h"
#include "../Views/View.h"

namespace developmentKit::library::uiFramework::presenters
{
    void ListPage::Init(View *prmView)
    {
        currentIndex = 0;
        itemSelected = false;
        view = prmView;
    }

    void ListPage::Increment()
    {
        if (itemSelected)
        {
            items[currentIndex]->Increment();
        }
        else
        {
            if (currentIndex < items.size() - 1)
            {
                currentIndex++;
            }
        }
    }

    void ListPage::Decrement()
    {
        if (itemSelected)
        {
            items[currentIndex]->Decrement();
        }
        else
        {
            if (currentIndex > 0)
            {
                currentIndex--;
            }
        }
    }

    void ListPage::Select()
    {
        items[currentIndex]->Select();
    }

    ListPageItem *ListPage::GetItem(uint8_t index)
    {
        return items[index];
    }

    void ListPage::AddItem(ListPageItem *pageItem)
    {
        items.push_back(pageItem);
    }

    uint8_t ListPage::ItemsCount()
    {
        return items.size();
    }

    uint8_t ListPage::GetCurrentIndex()
    {
        return currentIndex;
    }

    bool ListPage::GetItemSelected()
    {
        return itemSelected;
    }

    void ListPage::ToggleItemSelected()
    {
        itemSelected = !itemSelected;
    }

    void ListPage::Paint()
    {
        view->Paint(this);
    }
}