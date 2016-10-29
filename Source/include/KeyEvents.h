#pragma once

#include "Common.h"
#include <JuceHeader.h>

class KeyEvents
{
public:
        static void player_move_left(GameManager* gm, const int key_code);
        static void player_move_right(GameManager* gm, const int key_code);
        static void player_move_up(GameManager* gm, const int key_code);
        static void player_move_down(GameManager* gm, const int key_code);
        static void player_attack(GameManager* gm, const int key_code);
        static void player_set_current_item(GameManager* gm, const int key_code);

        static void open_inventory(GameManager* gm, const int key_code);
        static void close_inventory(GameManager* gm, const int key_code);

        static void inventory_left(GameManager* gm, const int key_code);
        static void inventory_right(GameManager* gm, const int key_code);
        static void inventory_up(GameManager* gm, const int key_code);
        static void inventory_down(GameManager* gm, const int key_code);
        static void select_item(GameManager* gm, const int key_code);
        static void drop_item(GameManager* gm, const int key_code);
        static void use_item(GameManager* gm, const int key_code);

        static void restart_game(GameManager* gm, const int key_code);
protected:
        static void move_selection_tile(GameManager* gm, juce::Point<int> dir);
};