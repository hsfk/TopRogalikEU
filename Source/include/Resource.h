#pragma once

#include <JuceHeader.h>
#include "CharacterSprite.h"
#include "Hud.h"

class Resource
{
public:
        static void initialize();
        static const juce::Image* get_empty_sprite();
        static const juce::Image* get_wall_sprite();
        static const juce::Image* get_ground_tile();
        static const juce::Image* get_small_heal_potion_sprite();
        static const juce::Image* get_medium_heal_potion_sprite();
        static const juce::Image* get_large_heal_potion_sprite();
        static const juce::Image* get_great_heal_potion_sprite();
        static const juce::Image* get_hurricane_sword_sprite();
        static const juce::Image* get_pyromancy_flame_sprite();
        static const juce::Image* get_dark_pyromancy_flame_sprite();
        static const juce::Image* get_darkfire_scroll_sprite();
        static const juce::Image* get_firestorm_scroll_sprite();
        static const juce::Image* get_blink_dagger_sprite();
        static const juce::Image* get_inventory_tile();
        static const juce::Image* get_inventory_selected_tile();
        static const juce::Image* get_inventory_tile_selection();
        static const Frames* get_bladevortex_frames();
        static const Frames* get_blood_frames();
        static const Frames* get_fireball_frames();
        static const Frames* get_dark_fireball_frames();
        static const Frames* get_fireball_explosion_frames();
        static const Frames* get_dark_fireball_explosion_frames();
        static const Frames* get_heal_frames();
        static const Frames* get_portal_frames();
        static const Frames* get_flickerstrike_frames();
        static const Frames* get_darkfire_frames();
        static const Frames* get_firestorm_frames();
        static const CharacterSprite* get_player_sprite();
        static const CharacterSprite* get_zombie_sprite();
        static const CharacterSprite* get_demon_sprite();
        static const CharacterSprite* get_princess_sprite();
        static const CharacterSprite* get_skeleton_sprite();
        static const CharacterSprite* get_dragon_sprite();
        static const Hud* get_hud();
protected:
        static Frames* player_sprite_asset;
        static Frames* zombie_sprite_asset;
        static Frames* demon_sprite_asset;
        static Frames* skeleton_sprite_asset;
        static Frames* princess_sprite_asset;
        static Frames* dragon_sprite_asset;

        static CharacterSprite* player_sprite;
        static CharacterSprite* zombie_sprite;
        static CharacterSprite* demon_sprite;
        static CharacterSprite* skeleton_sprite;
        static CharacterSprite* princess_sprite;
        static CharacterSprite* dragon_sprite;

        static Frames* bladevortex_asset_frames;
        static Frames* fireball_asset_frames;
        static Frames* dark_fireball_asset_frames;
        static Frames* blood_asset_frames;
        static Frames* fireball_explosion_asset_frames;
        static Frames* dark_fireball_explosion_asset_frames;
        static Frames* heal_asset_frames;
        static Frames* portal_asset_frames;
        static Frames* flickerstrike_asset_frames;
        static Frames* darkfire_asset_frames;
        static Frames* firestorm_asset_frames;

        static juce::Image* empty_sprite;
        static juce::Image* wall_frame;
        static juce::Image* ground_tile;
        static juce::Image* hud_left;
        static juce::Image* hud_right;
        static juce::Image* hud_health_orb;
        static juce::Image* hud_mana_orb;
        static juce::Image* small_heal_potion_sprite;
        static juce::Image* medium_heal_potion_sprite;
        static juce::Image* large_heal_potion_sprite;
        static juce::Image* great_heal_potion_sprite;
        static juce::Image* hurricane_sword_sprite;
        static juce::Image* pyromancy_flame_sprite;
        static juce::Image* dark_pyromancy_flame_sprite;
        static juce::Image* firestorm_scroll_sprite;
        static juce::Image* darkfire_scroll_sprite;
        static juce::Image* blink_dagger_sprite;
        static juce::Image* inventory_tile_sprite;
        static juce::Image* inventory_tile_selection_sprite;
        static juce::Image* inventory_selected_tile_sprite;

        static Hud* hud;
};
