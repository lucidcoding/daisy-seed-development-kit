#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#if defined(TESTING)

#include <iostream>
#include <string>
#include <sstream>

#define DEBUG(...)                        \
    {                                     \
        char buffer[255];                 \
        sprintf(buffer, __VA_ARGS__);     \
        std::cout << buffer << std::endl; \
    }

#elif defined(LOGGING)

#include "daisy_seed.h"

using namespace daisy;
extern DaisySeed daisySeed;

#define DEBUG(...)                    \
    {                                 \
        char buffer[255];             \
        sprintf(buffer, __VA_ARGS__); \
        daisySeed.PrintLine(buffer);  \
    }

#else
#define DEBUG(...)
#endif

#endif