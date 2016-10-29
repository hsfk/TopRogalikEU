#include "KeyEvents.h"
#include "GameManager.h"
#include "GlobalData.h"

#define player(gm) gm->gdata->get_map()->get_player()
#define pinventory(gm) player(gm)->get_inventory()

void KeyEvents::player_move_left(GameManager* gm, const int key_code)
{
        player(gm)->move_left();
        gm->next_cycle();
}

void KeyEvents::player_move_right(GameManager* gm, const int key_code)
{
        player(gm)->move_right();
        gm->next_cycle();
}

void KeyEvents::player_move_up(GameManager* gm, const int key_code)
{
        player(gm)->move_top();
        gm->next_cycle();
}

void KeyEvents::player_move_down(GameManager* gm, const int key_code)
{
        player(gm)->move_bottom();
        gm->next_cycle();
}

void KeyEvents::player_attack(GameManager* gm, const int key_code)
{
        if (!gm->stopped)
        {
                gm->next_cycle();
                player(gm)->attack();
        }
}

void KeyEvents::player_set_current_item(GameManager* gm, const int key_code)
{
        if (key_code > '0')
                player(gm)->set_current_item(key_code - '1');
        else
                player(gm)->set_current_item(pinventory(gm)->get_width() - 1);
}

void KeyEvents::open_inventory(GameManager* gm, const int key_code)
{
        gm->gdata->set_current_state(gm->states[GameManager::States::inventory]);
}

void KeyEvents::close_inventory(GameManager* gm, const int key_code)
{
        gm->gdata->set_current_state(gm->states[GameManager::States::game]);
}

void KeyEvents::inventory_left(GameManager* gm, const int key_code)
{
        move_selection_tile(gm, directions[Direction::dir_left]);
}

void KeyEvents::inventory_right(GameManager* gm, const int key_code)
{
        move_selection_tile(gm, directions[Direction::dir_right]);
}

void KeyEvents::inventory_up(GameManager* gm, const int key_code)
{
        move_selection_tile(gm, directions[Direction::dir_up]);
}

void KeyEvents::inventory_down(GameManager* gm, const int key_code)
{
        move_selection_tile(gm, directions[Direction::dir_down]);
}

void KeyEvents::select_item(GameManager* gm, const int key_code)
{
        if (pinventory(gm)->is_selected())
                pinventory(gm)->deselect();
        else
                pinventory(gm)->select();
}

void KeyEvents::drop_item(GameManager* gm, const int key_code)
{
        PlayerInventory* inv = pinventory(gm);
        Item* item = inv->get_item(inv->get_selection_tile());
        if (item)
        {
                player(gm)->drop_item(item);
                inv->set_item(nullptr, inv->get_selection_tile());
        }
}

void KeyEvents::use_item(GameManager* gm, const int key_code)
{
        PlayerInventory* inv = pinventory(gm);
        Item* item = inv->get_item(inv->get_selection_tile());
        if (item && inv->is_selected())
        {
                item->use(player(gm));
                inv->update();
                gm->next_cycle();
        }
}

void KeyEvents::restart_game(GameManager* gm, const int key_code)
{
        gm->gdata->get_map()->restore();
        gm->gdata->set_gdata();
        gm->gdata->set_current_state(gm->states[GameManager::States::game]);
        gm->gdata->set_game_over(false);
        gm->game_start();
}

void KeyEvents::move_selection_tile(GameManager* gm, juce::Point<int> dir)
{
        pinventory(gm)->move_selection_tile(dir);
}
