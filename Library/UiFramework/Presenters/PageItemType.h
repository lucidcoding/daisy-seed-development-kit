#pragma once
#ifndef PAGE_ITEM_TYPE_H
#define PAGE_ITEM_TYPE_H

// Seems to run into trouble if this is declared inside the presenters namespace?
enum PageItemType
{
    NAVIGATION_PAGE_ITEM,
    NUMERIC_SETTINGS_PAGE_ITEM,
    OPTIONS_SETTINGS_PAGE_ITEM
};

#endif