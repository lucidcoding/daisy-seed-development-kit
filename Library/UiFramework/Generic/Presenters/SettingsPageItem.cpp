#include "SettingsPageItem.h"
#include <string>

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    void SettingsPageItem::Select()
    {
        parent->ToggleItemSelected();
    }
}
