#include "SettingsListPageItem.h"
#include <string>

namespace developmentKit::library::uiFramework::presenters
{
    using namespace std;

    void SettingsListPageItem::Select()
    {
        parent->ToggleItemSelected();
    }
}
