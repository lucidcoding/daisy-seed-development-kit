#include <string>
#include "daisy_seed.h"
#include "daisysp.h"
#include "PotentiometerArrayPageView.h"
#include "../Presenters/PotentiometerArrayPage.h"
#include "../Presenters/PotentiometerArrayPageItem.h"
#include "../../../../ThirdParty/Daisy_ILI9394/ili9341_ui_driver.hpp"

namespace developmentKit::library::uiFramework::tree::view
{
    using namespace std;
    using namespace developmentKit::library::uiFramework::presenters;

    void PotentiometerArrayPageView::Init(ViewAdapter *prmViewAdapter, uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight)
    {
        viewAdapter = prmViewAdapter;
        x = prmX;
        y = prmY;
        width = prmWidth;
        height = prmHeight;
    }

    void PotentiometerArrayPageView::SetSize(uint16_t prmX, uint16_t prmY, uint16_t prmWidth, uint16_t prmHeight)
    {
        x = prmX;
        y = prmY;
        width = prmWidth;
        height = prmHeight;
    }

    void PotentiometerArrayPageView::Paint(Page *page)
    {
        PotentiometerArrayPage *potentiometerArrayPage = static_cast<PotentiometerArrayPage *>(page);
        const uint16_t cellHeight = height / 4;
        const uint16_t cellWidth = width / 4;

        // displayHardware->DrawRect(Rectangle(0, 0, 319, 239), COLOR_WHITE);
        // displayHardware->DrawRect(Rectangle(0, 0, menuWidth, 239), COLOR_WHITE);

        for (uint8_t i = 0; i < 16; i++)
        {
            PotentiometerArrayPageItem *item = potentiometerArrayPage->GetItem(i);

            if (item != NULL)
            {
                int16_t xOffset = ((i % 4) * cellWidth) + x;
                int16_t yOffset = ((i / 4) * cellHeight) + y;
                ViewAdapter::Color valueColor = item->GetKnobPositionAndOutputValueSynced() ? ViewAdapter::COLOR_WHITE : ViewAdapter::COLOR_GRAY;
                char buffer[25];
                strcpy(buffer, item->GetDisplayString().c_str());
                // displayHardware->WriteStringAligned(buffer, Font_11x18, Rectangle(xOffset, yOffset, cellWidth, cellHeight - 11), Alignment::centered, valueColor);
                //viewAdapter->WriteString(buffer, xOffset, yOffset, Font_11x18, valueColor);
                viewAdapter->WriteStringAligned(buffer, xOffset, yOffset, cellWidth, cellHeight - 11, Alignment::centered, Font_11x18, valueColor);
                strcpy(buffer, item->GetTitle().c_str());
                // displayHardware->WriteStringAligned(buffer, Font_6x8, Rectangle(xOffset, yOffset + cellHeight - 25, cellWidth, 25), Alignment::centered, COLOR_WHITE);
                //viewAdapter->WriteString(buffer, xOffset, yOffset, Font_6x8, ViewAdapter::COLOR_WHITE);
                viewAdapter->WriteStringAligned(buffer, xOffset, yOffset + cellHeight - 25, cellWidth, 25, Alignment::centered, Font_6x8, ViewAdapter::COLOR_WHITE);
            }
        }
    }
}