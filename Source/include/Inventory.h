#pragma once

#include "Items.h"
#include <vector>

class CharInventory
{
public:
        CharInventory(int size);

        bool has_free_space() const;
        int get_size() const;
        void update();
        void add_item(Item* item);
        Item* get_item(int idx) const;
        void set_item(Item* item, int idx);
protected:
        std::vector<Item*> backpack;
};

class PlayerInventory : public CharInventory
{
public:
        PlayerInventory();

        void move_selection_tile(juce::Point<int> dir);
        juce::Point<int> get_selection_tile() const;
        void select();
        void deselect();
        bool is_selected() const;
        Item* get_item(juce::Point<int> pos) const;
        void set_item(Item* item, juce::Point<int> pos);
        int get_width() const;
        int get_height() const;
protected:
        juce::Point<int> selection_tile;
        bool selected;
        std::vector<Item*> hotkeys;
};