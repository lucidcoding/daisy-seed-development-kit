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
        menuVisible = false;
    }

    void TabPage::Increment()
    {
        if(menuVisible)
        {
            menuContent->Increment();
            return;
        }

        items[currentIndex]->GetContent()->Increment();
    }

    void TabPage::Decrement()
    {
        if(menuVisible)
        {
            menuContent->Decrement();
            return;
        }
        
        items[currentIndex]->GetContent()->Decrement();
    }

    void TabPage::SetPotentiometerValues(float *values)
    {
        if(menuVisible)
        {
            menuContent->SetPotentiometerValues(values);
            return;
        }
        
        items[currentIndex]->GetContent()->SetPotentiometerValues(values);
    }

    void TabPage::Left()
    {
        if(menuVisible)
        {
            menuContent->Left();
            return;
        }
        
        if (currentIndex > 0)
        {
            currentIndex--;
        }
        
        items[currentIndex]->GetContent()->SetFocus();
    }

    void TabPage::Right()
    {
        if(menuVisible)
        {
            menuContent->Right();
            return;
        }
        
        if (currentIndex < items.size() - 1)
        {
            currentIndex++;
        }

        items[currentIndex]->GetContent()->SetFocus();
    }

    void TabPage::Menu()
    {
        menuVisible = !menuVisible;
    }

    void TabPage::Select()
    {
        if (menuVisible)
        {
            menuContent->Select();
        }

        items[currentIndex]->GetContent()->Select();
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

    bool TabPage::GetMenuVisible()
    {
        return menuVisible;
    }

    void TabPage::SetMenuContent(Page *newMenuContent)
    {
        menuContent = newMenuContent;
    }

    Page *TabPage::GetMenuContent()
    {
        return menuContent;
    }

    void TabPage::Paint()
    {
        view->Paint(this);
    }
}