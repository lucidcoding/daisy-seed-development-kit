#pragma once
#ifndef OPTION_H
#define OPTION_H

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    class Option
    {
    public:
        Option() {}
        ~Option() {}
        const char *title;
        uint8_t value;
    };
}

#endif