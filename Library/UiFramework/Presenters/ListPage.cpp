#include "ListPage.h"
#include "PageItem.h"
#include "Page.h"
#include "../View/ListPageView.h"

namespace developmentKit::library::uiFramework::presenters
{
    ListPage::ListPage(ListPageView *prmView)
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

    PageItem *ListPage::GetItem(unsigned int index)
    {
        return items[index];
    }

    void ListPage::AddItem(PageItem *pageItem)
    {
        items.push_back(pageItem);
    }

    unsigned int ListPage::ItemsCount()
    {
        return items.size();
    }

    unsigned int ListPage::GetCurrentIndex()
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