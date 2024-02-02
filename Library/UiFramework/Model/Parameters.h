#pragma once
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <vector>

namespace developmentKit::library::uiFramework::model
{
    class Parameters
    {
    public:
        Parameters();
        unsigned int value1;
        unsigned int value2;
        unsigned int value3;
        unsigned int effectType;

    private:
    };
}

#endif