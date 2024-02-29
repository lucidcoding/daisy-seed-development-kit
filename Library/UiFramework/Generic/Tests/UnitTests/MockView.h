#pragma once
#ifndef MOCK_VIEW_H
#define MOCK_VIEW_H

#include "../../Views/View.h"

namespace developmentKit::library::uiFramework::tests::unitTests
{
    using namespace developmentKit::library::uiFramework::views;

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