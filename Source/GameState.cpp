#include "GameState.h"
#include "Engine.h"
#include "GameManager.h"
#include "KeyEvents.h"

GameState::GameState(GameManager* gm) : gm(gm)
{
        layout = new KeyboardLayout<GameManager>(gm);
}

void GameState::key_press(const int key_code)
{
        layout->key_press(key_code);
}


const juce::Image* GameState::render()
{
        return Engine::render();
}

Game::Game(GameManager* gm)
        : GameState(gm)
{
        layout->add_keypress(juce::KeyPress::upKey, KeyEvents::player_move_up);
        layout->add_keypress(juce::KeyPress::downKey, KeyEvents::player_move_down);
        layout->add_keypress(juce::KeyPress::leftKey, KeyEvents::player_move_left);
        layout->add_keypress(juce::KeyPress::rightKey, KeyEvents::player_move_right);
        layout->add_keypress(juce::KeyPress::spaceKey, KeyEvents::player_attack);
        layout->add_keypress('I', KeyEvents::open_inventory);
        layout->add_keypress('0', '9', KeyEvents::player_set_current_item);
}

const juce::Image* Game::render()
{
        return Engine::render();
}

Inventory::Inventory(GameManager* gm)
        : GameState(gm)
{
        layout->add_keypress('I', KeyEvents::close_inventory);
        layout->add_keypress(juce::KeyPress::escapeKey, KeyEvents::close_inventory);
        
        layout->add_keypress(juce::KeyPress::upKey, KeyEvents::inventory_up);
        layout->add_keypress(juce::KeyPress::downKey, KeyEvents::inventory_down);
        layout->add_keypress(juce::KeyPress::leftKey, KeyEvents::inventory_left);
        layout->add_keypress(juce::KeyPress::rightKey, KeyEvents::inventory_right);
        layout->add_keypress(juce::KeyPress::spaceKey, KeyEvents::select_item);
        layout->add_keypress(juce::KeyPress::deleteKey, KeyEvents::drop_item);
        layout->add_keypress(13, KeyEvents::use_item); // enter
}

const juce::Image* Inventory::render()
{
        return Engine::render_inventory();
}

GameOver::GameOver(GameManager* gm)
        : GameState(gm)
{
        layout->add_keypress(13, KeyEvents::restart_game);
}

const juce::Image* GameOver::render()
{
        return Engine::render_gameover();
}
