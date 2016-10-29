#pragma once

#include "Items.h"

class DropManager
{
public:
        enum ItemRarity
        {
                common,
                magic,
                rare,
                unique
                // ...
        };

        typedef Item*(*ItemGetter)();

        static std::vector<Item*> get_drop(int iir, int iiq);
protected:
        static std::vector<ItemGetter> item_getters;

        static Item* get_item(int iir);
        static Item* get_common_item();
        static Item* get_magic_item();
        static Item* get_rare_item();
        static Item* get_unique_item();
};