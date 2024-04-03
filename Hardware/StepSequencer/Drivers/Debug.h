#pragma once
#ifndef DEBUG_H
#define DEBUG_H


#if defined(TESTING)
#include <iostream>
#define DEBUG(x) std::cout << x << "\n"
#elif defined(LOGGING)
#include <iostream>
#include <string>  
#include <sstream> 
#include "DaisySeedReference.h"
#define DEBUG(x) { \
    std::stringstream buffer; \
    buffer << x << std::endl;; \
    daisySeed.PrintLine(buffer); \
}
#else
#define DEBUG(x)
#endif

#endif