#pragma once

#include "Common.h"
#include "Map.h"

class GlobalState
{
public:
        GlobalState(GameMap* map);
        ~GlobalState() { delete map; }

        void set_game_over(bool game_over);
        bool is_game_over() const;
        unsigned get_move_counter() const;
        unsigned get_score() const;
        GameMap* get_map() const;

        GameState* get_current_state();
        void set_current_state(GameState* state);
        void set_gdata();
protected:

        GameMap* map;
        unsigned move_counter;
        unsigned score;
        GameState* current;
        bool game_over;
};