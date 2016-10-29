#include "Drop.h"
#include "Random.h"

std::vector<DropManager::ItemGetter> DropManager::item_getters =
{
        DropManager::get_common_item,
        DropManager::get_magic_item,
        DropManager::get_rare_item,
        DropManager::get_unique_item
};

std::vector<Item*> DropManager::get_drop(int iir, int iiq)
{
        std::vector<Item*> result;
        int rarity = (130 + iir) / 10;
        int quantity = (150 + iiq) / 10;
        if (quantity > 80)
                quantity = 80;
        if (rarity > 80)
                rarity = 80;

        if (RNG::next_uint() % 100 > quantity)
                return result; // no drop =(

        int item_count = 1;

        int rand = RNG::next_uint() % 100;
        while (RNG::next_uint() % 100 <= quantity)
                item_count++;  

        for (int i = 0; i < item_count; i++)
                result.push_back(get_item(rarity));

        return result;
}

Item* DropManager::get_item(int iir)
{
        int rarity = ItemRarity::common;
        while (RNG::next_uint() % 100 <= iir)
                rarity++;
        if (rarity > ItemRarity::unique)
                rarity = ItemRarity::unique;
        return item_getters[rarity]();
}

Item* DropManager::get_common_item()
{
        return new SmallHealPotion();
}

Item* DropManager::get_magic_item()
{
        if (RNG::next_bool())
                return new HurricaneSword();
        else
                return new MediumHealPotion();


}

Item* DropManager::get_rare_item()
{
        unsigned rand = RNG::next_uint() % 4;
        if (rand == 0)
                return new BlinkDagger();
        else if (rand == 1)
                return new PyromancyFlame();
        else if (rand == 2)
                return new LargeHealPotion();
        else
                return new DarkPyromancyFlame();
}

Item* DropManager::get_unique_item()
{
        unsigned rand = RNG::next_uint() % 3;
        if (rand == 0)
                return new DarkfireScroll();
        else if (rand == 1)
                return new GreatHealPotion();
        else
                return new FirestormScroll();
}
