#include "MockUiParameterProvider.h"

namespace developmentKit::library::uiFramework::tests
{
    MockUiParameterProvider::MockUiParameterProvider(int prmMin, int prmMax, int prmValue)
    {
        value = prmValue;
        min = prmMin;
        max = prmMax;
    }
}