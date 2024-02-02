#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "./Model/Parameters.h"
#include "./Presenters/Page.h"
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include <string>
#include <vector>

namespace developmentKit::library::uiFramework
{
    using namespace developmentKit::library::uiFramework::model;
    using namespace developmentKit::library::uiFramework::presenters;
    using namespace daisy;

    class Display
    {
    public:
        Display(OledDisplay<SSD130xI2c128x64Driver> *display, Parameters *prmParameters);
        void Increment();
        void Decrement();
        void Select();
        void SetPage(Page *newPage);
        Page *GetCurrentPage();
        void SetCurrentPage(Page *newCurrentPage);
        void Paint();

    private:
        Page *homePage;
        Parameters *parameters;
        Page *currentPage;
    };
}

#endif