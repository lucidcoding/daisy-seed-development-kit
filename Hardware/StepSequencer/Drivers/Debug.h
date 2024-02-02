#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#ifdef TESTING
#include <iostream>
#define DEBUG(x) std::cout << x << "\n"
#else
#define DEBUG(x)
#endif

#endif