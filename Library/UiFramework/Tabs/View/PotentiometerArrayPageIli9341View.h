#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_ILI9341_VIEW_H
#define POTENTIOMETER_ARRAY_PAGE_ILI9341_VIEW_H

#include "View.h"
#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tabs::view
{
    using namespace developmentKit::library::uiFramework::tabs::presenters;
    using namespace daisy;

    class PotentiometerArrayPageIli9341View : public View
    {
    public:
        ~PotentiometerArrayPageIli9341View() {}
        PotentiometerArrayPageIli9341View(UiDriver *prmDisplayHardware);
        PotentiometerArrayPageIli9341View(UiDriver *prmDisplayHardware, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight);
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
