#pragma once
#ifndef TAB_PAGE_ILI9341_VIEW_H
#define TAB_PAGE_ILI9341_VIEW_H

#include "View.h"
#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace developmentKit::library::uiFramework::tree::presenters;
    using namespace daisy;

    class TabPageIli9341View : public View
    {
    public:
        ~TabPageIli9341View() {}
        TabPageIli9341View(UiDriver *prmDisplayHardware);
        void Paint(Page *page);

    private:
        UiDriver *displayHardware;
    };
}

#endif
