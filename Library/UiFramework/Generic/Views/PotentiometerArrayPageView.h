#pragma once
#ifndef POTENTIOMETER_ARRAY_PAGE_VIEW_H
#define POTENTIOMETER_ARRAY_PAGE_VIEW_H

#include "View.h"
#include "../ViewAdapters/ViewAdapter.h"
#include "daisy_seed.h"
#include "daisysp.h"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace developmentKit::library::uiFramework::presenters;
    using namespace developmentKit::library::uiFramework::tree::viewAdapters;
    using namespace daisy;

    class PotentiometerArrayPageView : public View
    {
    public:
        PotentiometerArrayPageView() {}
        ~PotentiometerArrayPageView() {}
        void Init(ViewAdapter *prmViewAdapter, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight);
        void SetSize(uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight);
        void Paint(Page *page);

    private:
        ViewAdapter *viewAdapter;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
    };
}

#endif
