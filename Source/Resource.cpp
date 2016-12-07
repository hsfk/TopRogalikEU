#include "Resource.h"

#include "resources/skills/bladevortex_asset.h"
#include "resources/skills/fireball_asset.h"
#include "resources/skills/dark_fireball_asset.h"
#include "resources/skills/flickerstrike_asset.h"

#include "resources/effects/blood_asset.h"
#include "resources/effects/fireball_explosion_asset.h"
#include "resources/effects/dark_fireball_explosion_asset.h"
#include "resources/effects/heal_asset.h"
#include "resources/effects/darkfire_asset.h"
#include "resources/effects/firestorm_asset.h"

#include "resources/characters/knight_asset.h"
#include "resources/characters/zombie_asset.h"
#include "resources/characters/demon_asset.h"
#include "resources/characters/princess_asset.h"
#include "resources/characters/skeleton_asset.h"
#include "resources/characters/dragon_asset.h"

#include "resources/envieronment/wall_sprite.h"
#include "resources/envieronment/floor_sprite.h"
#include "resources/envieronment/portal_asset.h"

#include "resources/hud/hud_health_orb_sprite.h"
#include "resources/hud/hud_mana_orb_sprite.h"
#include "resources/hud/hud_sprite_left.h"
#include "resources/hud/hud_sprite_right.h"

#include "resources/items/small_heal_potion.h"
#include "resources/items/medium_heal_potion.h"
#include "resources/items/large_heal_potion.h"
#include "resources/items/great_heal_potion.h"
#include "resources/items/hurricane_sword.h"
#include "resources/items/pyromancy_flame.h"
#include "resources/items/dark_pyromancy_flame.h"
#include "resources/items/blink_dagger.h"
#include "resources/items/darkfire_scroll.h"
#include "resources/items/firestorm_scroll.h"

#include "resources/inventory/inventory_selected_tile.h"
#include "resources/inventory/inventory_tile.h"
#include "resources/inventory/inventory_tile_selection.h"

Frames* Resource::player_sprite_asset = nullptr;
Frames* Resource::zombie_sprite_asset = nullptr;
Frames* Resource::demon_sprite_asset = nullptr;
Frames* Resource::skeleton_sprite_asset = nullptr;
Frames* Resource::princess_sprite_asset = nullptr;
Frames* Resource::dragon_sprite_asset = nullptr;

CharacterSprite* Resource::player_sprite = nullptr;
CharacterSprite* Resource::zombie_sprite = nullptr;
CharacterSprite* Resource::demon_sprite = nullptr;
CharacterSprite* Resource::skeleton_sprite = nullptr;
CharacterSprite* Resource::princess_sprite = nullptr;
CharacterSprite* Resource::dragon_sprite = nullptr;

Frames* Resource::bladevortex_asset_frames = nullptr;
Frames* Resource::fireball_asset_frames = nullptr;
Frames* Resource::dark_fireball_asset_frames = nullptr;
Frames* Resource::blood_asset_frames = nullptr;
Frames* Resource::fireball_explosion_asset_frames = nullptr;
Frames* Resource::dark_fireball_explosion_asset_frames = nullptr;
Frames* Resource::heal_asset_frames = nullptr;
Frames* Resource::portal_asset_frames = nullptr;
Frames* Resource::flickerstrike_asset_frames = nullptr;
Frames* Resource::darkfire_asset_frames = nullptr;
Frames* Resource::firestorm_asset_frames = nullptr;

juce::Image* Resource::empty_sprite = nullptr;
juce::Image* Resource::wall_frame = nullptr;
juce::Image* Resource::ground_tile = nullptr;
juce::Image* Resource::inventory_tile_sprite = nullptr;
juce::Image* Resource::inventory_tile_selection_sprite = nullptr;
juce::Image* Resource::inventory_selected_tile_sprite = nullptr;

juce::Image* Resource::small_heal_potion_sprite = nullptr;
juce::Image* Resource::medium_heal_potion_sprite = nullptr;
juce::Image* Resource::large_heal_potion_sprite = nullptr;
juce::Image* Resource::great_heal_potion_sprite = nullptr;

juce::Image* Resource::hurricane_sword_sprite = nullptr;
juce::Image* Resource::blink_dagger_sprite = nullptr;
juce::Image* Resource::pyromancy_flame_sprite = nullptr;
juce::Image* Resource::dark_pyromancy_flame_sprite = nullptr;
juce::Image* Resource::darkfire_scroll_sprite = nullptr;
juce::Image* Resource::firestorm_scroll_sprite = nullptr;

juce::Image* Resource::hud_left = nullptr;
juce::Image* Resource::hud_right = nullptr;
juce::Image* Resource::hud_health_orb = nullptr;
juce::Image* Resource::hud_mana_orb = nullptr;
Hud* Resource::hud = nullptr;

#define get_img(img) ImageCache::getFromMemory(img, img##_size)

void Resource::initialize()
{
        player_sprite_asset = new Frames(get_img(knight_asset), 4);
        zombie_sprite_asset = new Frames(get_img(zombie_asset), 4);
        demon_sprite_asset = new Frames(get_img(demon_asset), 4);
        princess_sprite_asset = new Frames(get_img(princess_asset), 4);
        skeleton_sprite_asset = new Frames(get_img(skeleton_asset), 4);
        dragon_sprite_asset = new Frames(get_img(dragon_asset), 4);

        player_sprite = new CharacterSprite(player_sprite_asset);
        zombie_sprite = new CharacterSprite(zombie_sprite_asset);
        demon_sprite = new CharacterSprite(demon_sprite_asset);
        princess_sprite = new CharacterSprite(princess_sprite_asset);
        skeleton_sprite = new CharacterSprite(skeleton_sprite_asset);
        dragon_sprite = new CharacterSprite(dragon_sprite_asset);

        bladevortex_asset_frames = new Frames(get_img(bladevortex_asset), 8);
        blood_asset_frames = new Frames(get_img(blood_asset), 8);
        fireball_asset_frames = new Frames(get_img(fireball_asset), 16);
        dark_fireball_asset_frames = new Frames(get_img(dark_fireball_asset), 16);
        fireball_explosion_asset_frames = new Frames(get_img(fireball_explosion_asset), 8, 6);
        dark_fireball_explosion_asset_frames = new Frames(get_img(dark_fireball_explosion_asset), 8, 8);
        heal_asset_frames = new Frames(get_img(heal_asset), 19);
        portal_asset_frames = new Frames(get_img(portal_asset), 4);
        flickerstrike_asset_frames = new Frames(get_img(flickerstrike_asset), 5, 6);
        darkfire_asset_frames = new Frames(get_img(darkfire_asset), 5, 5);
        firestorm_asset_frames = new Frames(get_img(firestorm_asset), 20);

        empty_sprite = new Image();
        wall_frame = new Image(get_img(wall_sprite));
        ground_tile = new Image(get_img(floor_sprite));

        hud_left = new Image(get_img(hud_sprite_left));
        hud_right = new Image(get_img(hud_sprite_right));
        hud_health_orb = new Image(get_img(hud_health_orb_sprite));
        hud_mana_orb = new Image(get_img(hud_mana_orb_sprite));
        hud = new Hud(hud_left, hud_right, hud_health_orb, hud_mana_orb);

        inventory_tile_selection_sprite = new Image(get_img(inventory_tile_selection));
        inventory_selected_tile_sprite = new Image(get_img(inventory_selected_tile));

        small_heal_potion_sprite = new Image(get_img(small_heal_potion));
        medium_heal_potion_sprite = new Image(get_img(medium_heal_potion));
        large_heal_potion_sprite = new Image(get_img(large_heal_potion));
        great_heal_potion_sprite = new Image(get_img(great_heal_potion));

        hurricane_sword_sprite = new Image(get_img(hurricane_sword));
        pyromancy_flame_sprite = new Image(get_img(pyromancy_flame));
        dark_pyromancy_flame_sprite = new Image(get_img(dark_pyromancy_flame));
        inventory_tile_sprite = new Image(get_img(inventory_tile));
        blink_dagger_sprite = new Image(get_img(blink_dagger));
        darkfire_scroll_sprite = new Image(get_img(darkfire_scroll));
        firestorm_scroll_sprite = new Image(get_img(firestorm_scroll));

        ImageCache::releaseUnusedImages();
}

const juce::Image* Resource::get_empty_sprite()
{
        return empty_sprite;
}

const juce::Image* Resource::get_wall_sprite()
{
        return wall_frame;
}

const juce::Image* Resource::get_ground_tile()
{
        return ground_tile;
}

const juce::Image* Resource::get_small_heal_potion_sprite()
{
        return small_heal_potion_sprite;
}

const juce::Image* Resource::get_medium_heal_potion_sprite()
{
        return medium_heal_potion_sprite;
}

const juce::Image* Resource::get_large_heal_potion_sprite()
{
        return large_heal_potion_sprite;
}

const juce::Image* Resource::get_great_heal_potion_sprite()
{
        return great_heal_potion_sprite;
}

const juce::Image* Resource::get_hurricane_sword_sprite()
{
        return hurricane_sword_sprite;
}

const juce::Image* Resource::get_pyromancy_flame_sprite()
{
        return pyromancy_flame_sprite;
}

const juce::Image* Resource::get_dark_pyromancy_flame_sprite()
{
        return dark_pyromancy_flame_sprite;
}

const juce::Image* Resource::get_darkfire_scroll_sprite()
{
        return darkfire_scroll_sprite;
}

const juce::Image* Resource::get_firestorm_scroll_sprite()
{
        return firestorm_scroll_sprite;
}

const juce::Image* Resource::get_blink_dagger_sprite()
{
        return blink_dagger_sprite;
}

const juce::Image* Resource::get_inventory_tile()
{
        return inventory_tile_sprite;
}

const juce::Image* Resource::get_inventory_selected_tile()
{
        return inventory_selected_tile_sprite;
}

const juce::Image* Resource::get_inventory_tile_selection()
{
        return inventory_tile_selection_sprite;
}

const Frames* Resource::get_bladevortex_frames()
{
        return bladevortex_asset_frames;
}

const Frames* Resource::get_blood_frames()
{
        return blood_asset_frames;
}

const Frames* Resource::get_fireball_frames()
{
        return fireball_asset_frames;
}

const Frames* Resource::get_dark_fireball_frames()
{
        return dark_fireball_asset_frames;
}

const Frames* Resource::get_fireball_explosion_frames()
{
        return fireball_explosion_asset_frames;
}

const Frames* Resource::get_dark_fireball_explosion_frames()
{
        return dark_fireball_explosion_asset_frames;;
}

const Frames* Resource::get_heal_frames()
{
        return heal_asset_frames;
}

const Frames* Resource::get_portal_frames()
{
        return portal_asset_frames;
}

const Frames* Resource::get_flickerstrike_frames()
{
        return flickerstrike_asset_frames;
}

const Frames* Resource::get_darkfire_frames()
{
        return darkfire_asset_frames;
}

const Frames* Resource::get_firestorm_frames()
{
        return firestorm_asset_frames;
}

const CharacterSprite* Resource::get_player_sprite()
{
        return player_sprite;
}

const CharacterSprite* Resource::get_zombie_sprite()
{
        return zombie_sprite;
}

const CharacterSprite* Resource::get_demon_sprite()
{
        return demon_sprite;
}

const CharacterSprite* Resource::get_princess_sprite()
{
        return princess_sprite;
}

const CharacterSprite* Resource::get_skeleton_sprite()
{
        return skeleton_sprite;
}

const CharacterSprite* Resource::get_dragon_sprite()
{
        return dragon_sprite;
}

const Hud* Resource::get_hud()
{
        return hud;
}
