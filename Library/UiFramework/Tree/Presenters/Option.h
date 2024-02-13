#pragma once
#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>

using namespace std;

namespace developmentKit::library::uiFramework::tree::presenters
{
    class Option
    {
    public:
        Option() {}
        ~Option() {}
        string title;
        unsigned int value;
    };
}

#endif