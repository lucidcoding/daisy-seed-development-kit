#include "Sync.h"
#include "daisysp.h"
#include "daisy_seed.h"

namespace developmentKit::hardware::sync::drivers
{
    using namespace daisysp;
    using namespace daisy;

    void Sync::Init(DaisySeed *hardware)
    {
        syncOutput.pin = hardware->GetPin(24);
        syncOutput.mode = DSY_GPIO_MODE_OUTPUT_PP;
        syncOutput.pull = DSY_GPIO_PULLUP;
        dsy_gpio_init(&syncOutput);
        syncInput.pin = hardware->GetPin(25);
        syncInput.mode = DSY_GPIO_MODE_INPUT;
        syncInput.pull = DSY_GPIO_PULLUP;
        dsy_gpio_init(&syncInput);
        lastInputReading = false;
    }

    bool Sync::Poll()
    {
        bool returnValue = false;
        bool currentInputReading = !dsy_gpio_read(&syncInput);

        if(currentInputReading && !lastInputReading)
        {
            returnValue = true;
        }

        lastInputReading = currentInputReading;
        return returnValue;
    }
}