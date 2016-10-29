#include "Inventory.h"

CharInventory::CharInventory(int size)
{
        backpack.resize(size, nullptr);
}

bool CharInventory::has_free_space() const
{
        for (size_t i = 0; i < backpack.size(); i++)
                if (backpack[i] == nullptr)
                        return true;
        return false;
}

int CharInventory::get_size() const
{
        return backpack.size();
}

void CharInventory::update()
{
        for (size_t i = 0; i < backpack.size(); i++)
                if (backpack[i] && backpack[i]->get_count() <= 0)
                {
                        delete backpack[i];
                        backpack[i] = nullptr;
                }
}

void CharInventory::add_item(Item* item)
{
        for (size_t i = 0; i < backpack.size(); i++)
                if (backpack[i] && backpack[i]->get_id() == item->get_id() && backpack[i]->is_stackable())
                {
                        backpack[i]->set_count(backpack[i]->get_count() + item->get_count());
                        delete item;
                        return;
                }
        for (size_t i = 0; i < backpack.size(); i++)
                if (!backpack[i])
                {
                        backpack[i] = item;
                        return;
                }
}

Item* CharInventory::get_item(int idx) const
{
        return backpack[idx];
}

void CharInventory::set_item(Item* item, int idx)
{
        backpack[idx] = item;
}

PlayerInventory::PlayerInventory()
        : CharInventory(60)
{
        hotkeys.resize(10, nullptr);
        selected = false;
}

void PlayerInventory::move_selection_tile(juce::Point<int> dir)
{
        juce::Point<int> new_pos = selection_tile + dir;
        new_pos.x %= get_width();
        new_pos.y %= get_height();
        new_pos.x = new_pos.x < 0 ? get_width() - 1 : new_pos.x;
        new_pos.y = new_pos.y < 0 ? get_height() - 1 : new_pos.y;

        if (selected)
        {
                Item* tmp = get_item(new_pos);
                set_item(get_item(selection_tile), new_pos);
                set_item(tmp, selection_tile);
        }
        selection_tile = new_pos;
}

juce::Point<int> PlayerInventory::get_selection_tile() const
{
        return selection_tile;
}

void PlayerInventory::select()
{
        selected = true;
}

void PlayerInventory::deselect()
{
        selected = false;
}

bool PlayerInventory::is_selected() const
{
        return selected;
}

Item* PlayerInventory::get_item(juce::Point<int> pos) const
{
        return CharInventory::get_item(pos.x + pos.y * get_width());
}

void PlayerInventory::set_item(Item* item, juce::Point<int> pos)
{
        CharInventory::set_item(item, pos.x + pos.y * get_width());
}

int PlayerInventory::get_width() const
{
        return 10;
}

int PlayerInventory::get_height() const
{
        return backpack.size() / get_width();
}
