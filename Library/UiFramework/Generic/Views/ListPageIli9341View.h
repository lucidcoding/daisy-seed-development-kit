#pragma once
#ifndef LIST_PAGE_ILI9341_VIEW_H
#define LIST_PAGE_ILI9341_VIEW_H

#include "View.h"
#include "../Presenters/ListPage.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::views
{
    using namespace developmentKit::library::uiFramework::presenters;

    class ListPageIli9341View : public View
    {
    public:
        ~ListPageIli9341View() {}
        ListPageIli9341View(UiDriver *prmDisplayHardware);
        ListPageIli9341View(UiDriver *prmDisplayHardware, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight);
        void Paint(Page *page);

    private:
        UiDriver *displayHardware;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
    };
}

#endif
