#pragma once
#ifndef LIST_PAGE_VIEW_H
#define LIST_PAGE_VIEW_H

#include "View.h"
#include "../Presenters/ListPage.h"
#include "../ViewAdapters/ViewAdapter.h"
#include "daisy_seed.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace developmentKit::library::uiFramework::presenters;
    using namespace developmentKit::library::uiFramework::tree::viewAdapters;
    using namespace daisy;

    class ListPageView : public View
    {
    public:
        ~ListPageView() {}
        ListPageView() {}
        void Init(ViewAdapter *prmViewAdapter);
        void Paint(Page *page);

    private:
        ViewAdapter *viewAdapter;
    };
}

#endif
