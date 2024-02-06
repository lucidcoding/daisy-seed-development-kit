#pragma once
#ifndef LIST_PAGE_VIEW_H
#define LIST_PAGE_VIEW_H

#include "View.h"
#include "../Presenters/ListPage.h"
#include "daisy_seed.h"
#include "daisysp.h"
#include "dev/oled_ssd130x.h"

namespace developmentKit::library::uiFramework::view
{
    using namespace developmentKit::library::uiFramework::presenters;
    using namespace daisy;

    class ListPageView : public View
    {
    public:
        ListPageView() {}
        ~ListPageView() {}
        ListPageView(OledDisplay<SSD130xI2c128x64Driver> *prmDisplay);
        void SetListPage(ListPage *prmListPage);
        void Paint();

    private:
        OledDisplay<SSD130xI2c128x64Driver> *display;
        ListPage *listPage;
    };
}

#endif