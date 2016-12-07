#pragma once

#include "Map.h"
#include "Animation.h"
#include "Resource.h"
#include "Inventory.h"

class Engine
{
public:
        static void initialize(const GameMap* map, int width, int height, int cell_size);
        static const Image* render();
        static const Image* render_inventory();
        static const Image* render_gameover();
        static int get_width();
        static int get_height();
        static juce::Rectangle<int> get_camera_range();
protected:
        static Image* create_empty_image();

        static void render_obj(const game_object::GameObject* obj, const juce::Point<int> pos);
        static void render_obj_sprite(const game_object::GameObject* obj, juce::Point<int> pos);
        static void render_sprite_centrified(const juce::Image* sprite, juce::Point<int> pos);
        static void render_sprite_bottom_aligned(const juce::Image* sprite, juce::Point<int> pos);
        static void render_obj_state(const game_object::GameObject* obj, juce::Point<int> top_left);

        static void render_submap(const juce::Rectangle<int> region);
        static void render_hud();
        static void render_hud_left();
        static void render_hud_right();
  
        static void render_obstacle(const game_object::StaticObject* obj, const juce::Point<int> pos);
        static void render_ground(const juce::Rectangle<int> region);
        static void render_other(const juce::Rectangle<int> region);
        static void render_temp_obj_list(std::list<game_object::TempObject*> list, const juce::Point<int> pos);
        static void render_animation_list(AnimationList* list, const juce::Point<int> pos);

        static void render_hotkey_tab();
        static void render_backpack();
        static void render_item(const Item* item, juce::Point<int> pixel_pos, bool selection_tile, bool selected);

        static Image* buffer;
        static Graphics* gbuffer;
        static const GameMap* map;
        static const Hud* hud;
        static int width;
        static int height;
        static int cell_size;
};
