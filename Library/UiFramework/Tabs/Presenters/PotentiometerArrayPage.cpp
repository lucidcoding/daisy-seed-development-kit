#include "PotentiometerArrayPage.h"
#include "../View/View.h"

namespace developmentKit::library::uiFramework::tabs::presenters
{
    PotentiometerArrayPage::PotentiometerArrayPage(View *prmView)
    {
        view = prmView;
    }

    void PotentiometerArrayPage::Increment()
    {
    }

    void PotentiometerArrayPage::Decrement()
    {
    }

    void PotentiometerArrayPage::SetPotentiometerValue(uint8_t index, float value)
    {
        items[index].SetRawValue(value);
    }

    void PotentiometerArrayPage::SetPotentiometerValues(float *values)
    {
        for(uint8_t i = 0; i < 16; i++)
        {
            items[i].SetRawValue(values[i]);
        }
    }

    void PotentiometerArrayPage::Select()
    {
    }

    void PotentiometerArrayPage::Paint()
    {
        view->Paint(this);
    }

    PotentiometerArrayPageItem *PotentiometerArrayPage::GetItem(uint8_t index)
    {
        return &(items[index]);
    }
}