#include <stdint.h>
#include "PotentiometerArrayPage.h"
#include "../Views/View.h"

namespace developmentKit::library::uiFramework::presenters
{
    void PotentiometerArrayPage::Init(View *prmView)
    {
        view = prmView;

        for (uint8_t index = 0; index < 16; index++)
        {
            items[index] = NULL;
        }
    }

    void PotentiometerArrayPage::Increment()
    {
    }

    void PotentiometerArrayPage::Decrement()
    {
    }

    void PotentiometerArrayPage::SetPotentiometerValues(float *values)
    {
        for (uint8_t index = 0; index < 16; index++)
        {
            if (items[index] != NULL)
            {
                items[index]->SetCurrentKnobPosition(values[index]);
            }
        }
    }

    void PotentiometerArrayPage::Select()
    {
    }

    void PotentiometerArrayPage::SetFocus()
    {
        for (uint8_t index = 0; index < 16; index++)
        {
            if (items[index] != NULL)
            {
                items[index]->SetFocus();
            }
        }
    }

    void PotentiometerArrayPage::SetOutputValues(float *values)
    {
        for (uint8_t index = 0; index < 16; index++)
        {
            if (items[index] != NULL)
            {
                items[index]->SetOutputValue(values[index]);
            }
        }
    }

    void PotentiometerArrayPage::Paint()
    {
        view->Paint(this);
    }

    PotentiometerArrayPageItem *PotentiometerArrayPage::GetItem(uint8_t index)
    {
        return items[index];
    }

    void PotentiometerArrayPage::SetItem(uint8_t index, PotentiometerArrayPageItem *item)
    {
        items[index] = item;
    }

    void PotentiometerArrayPage::SetKnobMode(PotentiometerArrayPageItem::KnobMode prmKnobMode)
    {
        for (uint8_t index = 0; index < 16; index++)
        {
            if (items[index] != NULL)
            {
                items[index]->SetKnobMode(prmKnobMode);
            }
        }
    }
}