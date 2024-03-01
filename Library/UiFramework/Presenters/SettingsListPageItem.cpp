#include "SettingsListPageItem.h"

namespace developmentKit::library::uiFramework::presenters
{
    void SettingsListPageItem::Select()
    {
        parent->ToggleItemSelected();
    }
}
