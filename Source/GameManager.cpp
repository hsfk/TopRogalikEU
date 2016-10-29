#include "GameManager.h"
#include "GlobalData.h"
#include "GameState.h"
#include "Engine.h"
#include "Settings.h"
#include <list>

GameManager::GameManager(MainContentComponent* main_window, GlobalState* gdata)
        :main_window(main_window), gdata(gdata)
{
        this->main_window->setWantsKeyboardFocus(true);
        this->main_window->addKeyListener(this);

        states.push_back(new Game(this));
        states.push_back(new Inventory(this));
        states.push_back(new GameOver(this));
        this->gdata->set_current_state(states[States::game]);
        game_stop();
}

GameManager::~GameManager()
{
        delete gdata;
}

void GameManager::game_start()
{
        stopped = false;
        startTimer(1000);
}

void GameManager::game_stop()
{
        stopped = true;
        stopTimer();
}

bool GameManager::keyPressed(const juce::KeyPress& key, Component* origin)
{
        gdata->get_current_state()->key_press(key.getKeyCode());
        return false;
}

void GameManager::timerCallback()
{
        if (Settings::Game::is_pseudo_real_time())
                next_cycle();
}

void GameManager::next_cycle()
{
        if (stopped || gdata->is_game_over())
                return;

        GameMap& map = *gdata->get_map();
        
        std::list<GameMap::Tile*> tiles = map.get_dyn_obj_tile_list();
        std::list<GameMap::Tile*>::iterator it = tiles.begin();
        while (it != tiles.end())
        {
                GameMap::Tile* tile = (*it++);
                if (tile->dynamic_obj)
                        tile->dynamic_obj->move();
        }
        RectIterator<int> i = Engine::get_camera_range();
        while (!i.end())
        {
                juce::Point<int> pos = (*i++);
                if (!map.has_temp_obj(pos))
                        continue;
                std::list<game_object::TempObject*>& list = map.get_temp_objs(pos);
                std::list<game_object::TempObject*>::iterator i = list.begin();
                while (i != list.end())
                        (*i++)->move();
        }
        if (gdata->is_game_over())
        {
                game_stop();
                gdata->set_current_state(states[game_over]);
                return;
        }
}