#pragma once
#ifndef VIEW_H
#define VIEW_H

#include "../Presenters/Page.h"

namespace developmentKit::library::uiFramework::view
{
    using namespace developmentKit::library::uiFramework::presenters;

    class View
    {
    public:
        virtual void Paint(Page *page) = 0;

    private:
    };
}

#endif