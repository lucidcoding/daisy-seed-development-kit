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

    class ListPageSsd1306I2cView : public View
    {
    public:
        ~ListPageSsd1306I2cView() {}
        ListPageSsd1306I2cView(OledDisplay<SSD130xI2c128x64Driver> *prmDisplay);
        void Paint(Page *page);

    private:
        OledDisplay<SSD130xI2c128x64Driver> *display;
    };
}

#endif
