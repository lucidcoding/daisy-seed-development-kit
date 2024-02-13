#pragma once
#ifndef MOCK_VIEW_H
#define MOCK_VIEW_H

#include "../../View/View.h"

namespace developmentKit::library::uiFramework::tabs::tests
{
    using namespace developmentKit::library::uiFramework::tabs::view;

    class MockView : public View
    {
    public:
        MockView() {}
        ~MockView() {}
        void Paint(Page *page) {}

    private:
    };
}

#endif