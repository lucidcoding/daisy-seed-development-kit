#include "TabPage.h"
#include "Page.h"
#include "../View/View.h"

namespace developmentKit::library::uiFramework::tree::presenters
{
    TabPage::TabPage(View *prmView)
    {
        currentIndex = 0;
        itemSelected = false;
        view = prmView;
    }

    void TabPage::Increment()
    {
    }

    void TabPage::Decrement()
    {
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

    void TabPage::Paint()
    {
        view->Paint(this);
        //items[currentIndex]->GetContent()->Paint();
    }
}