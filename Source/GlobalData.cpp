#include "GlobalData.h"
#include "StaticObject.h"
#include "DynamicObject.h"

GlobalState::GlobalState(GameMap* map) : move_counter(0), score(0), game_over(false)
{
        this->map = map;
        if (!map)
                return;
        set_gdata();
        current = nullptr;
}

void GlobalState::set_game_over(bool game_over)
{
        this->game_over = game_over;
}

bool GlobalState::is_game_over() const
{
        return game_over;
}

unsigned GlobalState::get_move_counter() const
{
        return move_counter;
}

unsigned GlobalState::get_score() const
{
        return score;
}

GameMap* GlobalState::get_map() const
{
        return map;
}

GameState* GlobalState::get_current_state()
{
        return current;
}

void GlobalState::set_current_state(GameState* state)
{
        current = state;
}

void GlobalState::set_gdata()
{
        for (unsigned i = 0; i < map->get_height(); i++)
                for (unsigned j = 0; j < map->get_width(); j++)
                {
                        GameMap::Tile* tile = (*map)[i][j];
                        if (tile)
                        {
                                if (tile->static_obj)
                                        tile->static_obj->set_gdata(this);
                                if (tile->dynamic_obj)
                                        tile->dynamic_obj->set_gdata(this);
                        }
                }
}
