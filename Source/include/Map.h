#pragma once

#include <JuceHeader.h>
#include "GameObject.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "TempObject.h"
#include "Animation.h"

const juce::Point<int> directions[8] = {
        juce::Point<int>( 1,  0),
        juce::Point<int>(-1,  0),
        juce::Point<int>( 0,  1),
        juce::Point<int>( 0, -1),

        juce::Point<int>(-1, -1),
        juce::Point<int>(-1,  1),
        juce::Point<int>( 1, -1),
        juce::Point<int>( 1,  1)
};

enum Direction
{
        dir_right,
        dir_left,
        dir_down,
        dir_up,

        dir_top_left,
        dir_bot_left,
        dir_top_right,
        dir_bot_right
};

template <typename T>
class RectIterator
{
public:
        RectIterator() : pos(), rect() { }
        RectIterator(juce::Rectangle<T> r) : rect(r) { pos = rect.getTopLeft(); }

        bool end() const
        {
                return pos == rect.getBottomRight();
        }

        RectIterator& operator=(const juce::Rectangle<T> r)
        {
                rect = r;
                pos = rect.getTopLeft();
        }

        RectIterator& operator++(int)
        {
                if (end())
                        return *this;
                if (pos.x < rect.getTopRight().x)
                        pos.x++;
                else
                {
                        pos.x = rect.getTopLeft().x;
                        pos.y++;
                }
                return *this;
        }

        juce::Point<T> operator*() const
        {
                return pos;
        }

protected:
        juce::Rectangle<T> rect;
        juce::Point<T> pos;
};

class GameMap
{
public:
        struct Tile
        {
                Tile() : static_obj(nullptr), dynamic_obj(nullptr), temp_objs()
                        { animations = new AnimationList(true); }

                game_object::StaticObject* static_obj;
                std::list<game_object::TempObject*> temp_objs;
                game_object::DynamicObject* dynamic_obj;
                AnimationList* animations;
        };

        class Row
        {
        public:
                Row(std::vector<Tile*> row) : row(row) { }
                Tile* operator[] (unsigned i) const
                {
                        return row[i];
                }
                std::vector<Tile*> row;
        };

        Row operator[](unsigned i) const
        {
                return Row(map[i]);
        }

        GameMap(unsigned width, unsigned height);
        GameMap(std::vector<std::string> data);
        ~GameMap();

        unsigned get_width() const;
        unsigned get_height() const;
        bool in_range(juce::Point<int> pos) const;
        bool has_static_obj(juce::Point<int> pos) const;
        bool has_dyn_obj(juce::Point<int> pos) const;
        bool has_temp_obj(juce::Point<int> pos) const;
        bool free_to_move(juce::Point<int> pos) const;
        void restore();
        void set_player(game_object::Knight* player);
        void add_animation(Animation* anim, juce::Point<int> pos);
        void add_temp_obj(game_object::TempObject* temp);
        void set_tile(unsigned i, unsigned j, Tile* cell);
        void set_princess(game_object::Princess* princess);
        game_object::Knight* get_player() const;
        game_object::Princess* get_princess() const;
        game_object::DynamicObject* get_dyn_obj(juce::Point<int> pos) const;
        std::list<Tile*> get_dyn_obj_tile_list() const;
        std::list<game_object::DynamicObject*> get_visible_dyn_obj_list() const;
        std::list<game_object::TempObject*>& get_temp_objs(juce::Point<int> pos) const;
        game_object::StaticObject* get_static_obj(juce::Point<int> pos) const;
        std::vector<juce::Point<int>> get_route(juce::Point<int> from, juce::Point<int> to) const;
        Tile* get_tile(juce::Point<int> pos) const;
protected:
        void add_game_obj(char c, juce::Point<int> pos);
        void add_dyn_obj(game_object::DynamicObject* obj);
        void clear_portals();

        void resize(unsigned width, unsigned height);
        void delete_objs();
        void set_width(unsigned width);
        void set_height(unsigned height);

        std::vector<std::string> orig;
        std::vector<game_object::Portal*> portals;

        std::vector<std::vector<Tile*>> map;
        game_object::Knight* player;
        game_object::Princess* princess;
        unsigned width;
        unsigned height;
};