#pragma once
#ifndef OPTION_H
#define OPTION_H

#include <string>

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    class Option
    {
    public:
        Option() {}
        ~Option() {}
        string title;
        uint8_t value;
    };
}

#endif