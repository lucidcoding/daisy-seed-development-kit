#pragma once
#ifndef PAGE_ITEM_TYPE_H
#define PAGE_ITEM_TYPE_H

// Seems to run into trouble if this is declared inside the presenters namespace?
enum PageItemType
{
    NavigationPageItem,
    NumericSettingsPageItem,
    OptionsSettingsPageItem
};

#endif