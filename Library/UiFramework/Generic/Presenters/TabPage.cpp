#include "Page.h"
#include "TabPage.h"
#include "../Views/View.h"

namespace developmentKit::library::uiFramework::presenters
{
    void TabPage::Init(View *prmView)
    {
        currentIndex = 0;
        itemSelected = false;
        view = prmView;
    }

    void TabPage::Increment()
    {
        if (currentIndex < items.size() - 1)
        {
            currentIndex++;
        }

        items[currentIndex]->GetContent()->SetFocus();
    }

    void TabPage::Decrement()
    {
        if (currentIndex > 0)
        {
            currentIndex--;
        }
        
        items[currentIndex]->GetContent()->SetFocus();
    }

    void TabPage::SetPotentiometerValues(float *values)
    {
        items[currentIndex]->GetContent()->SetPotentiometerValues(values);
    }

    void TabPage::Select()
    {
    }

    TabPageItem *TabPage::GetItem(uint8_t index)
    {
        return items[index];
    }

    void TabPage::AddItem(TabPageItem *item)
    {
        items.push_back(item);
    }

    uint8_t TabPage::ItemsCount()
    {
        return items.size();
    }

    uint8_t TabPage::GetCurrentIndex()
    {
        return currentIndex;
    }

    void TabPage::Paint()
    {
        view->Paint(this);
    }
}