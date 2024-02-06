#include "SettingsPageItem.h"
#include <string>

using namespace std;

namespace developmentKit::library::uiFramework::presenters
{
    void SettingsPageItem::Select()
    {
        parent->ToggleItemSelected();
    }
}
