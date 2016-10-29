#include "Engine.h"
#include "GlobalData.h"
#include "Settings.h"

Image* Engine::buffer = nullptr;
Graphics* Engine::gbuffer = nullptr;
const GameMap* Engine::map = nullptr;
const Hud* Engine::hud = nullptr;
int Engine::width = 0;
int Engine::height = 0;
int Engine::cell_size = 0;

void Engine::initialize(const GameMap* map, int width, int height, int cell_size)
{
        Engine::map = map;
        Engine::width = width;
        Engine::height = height;
        Engine::cell_size = cell_size;
        hud = Resource::get_hud();
        buffer = create_empty_image();
        gbuffer = new Graphics(*buffer);
}

const Image* Engine::render()
{
        gbuffer->fillAll(Colours::black);
        render_submap(get_camera_range());
        render_hud();
        render_hotkey_tab();
        return buffer;
}

const Image* Engine::render_inventory()
{
        render();
        gbuffer->fillAll(juce::Colour::fromRGBA(0, 0, 0, 180));
        render_backpack();
        return buffer;
}

const Image* Engine::render_gameover()
{
        gbuffer->fillAll(Colours::black);
        gbuffer->setColour(Colours::white);
        const juce::String msg("GAME OVER, press ENTER to restart...");
        gbuffer->drawSingleLineText(msg,
                (buffer->getWidth() - gbuffer->getCurrentFont().getStringWidth(msg)) / 2, buffer->getHeight() / 2);
        return buffer;
}

void Engine::render_hotkey_tab()
{
        int tile_size = Resource::get_inventory_tile()->getWidth() + 1;
        const PlayerInventory* inv = map->get_player()->get_inventory();
        juce::Point<int> off((buffer->getWidth() - cell_size * inv->get_width()) / 2,
                buffer->getHeight() - 100);
        for (int i = 0; i < inv->get_width(); i++)
        {
                juce::Point<int> pos(i, 0);
                render_item(inv->get_item(pos), off + pos * tile_size,
                        map->get_player()->get_current_item() == i, false);
        }
}

void Engine::render_backpack()
{
        int tile_size = Resource::get_inventory_tile()->getWidth() + 1;
        const PlayerInventory* inv = map->get_player()->get_inventory();
        juce::Point<int> off((buffer->getWidth() - tile_size * inv->get_width()) / 2,
                (buffer->getHeight() - tile_size * inv->get_height()) / 2);
        for (int i = 0; i < inv->get_height(); i++)
                for (int j = 0; j < inv->get_width(); j++)
                {
                        juce::Point<int> pos(j, i);
                        render_item(inv->get_item(pos),
                                off + pos * tile_size,
                                pos == inv->get_selection_tile(),
                                inv->is_selected());
                }
}

void Engine::render_item(const Item* item, juce::Point<int> pixel_pos, bool selection_tile, bool selected)
{
        const Image* tile = Resource::get_inventory_tile();
        if (selection_tile)
                tile = selected ? Resource::get_inventory_selected_tile()
                        : Resource::get_inventory_tile_selection();
        gbuffer->drawImageAt(*tile, pixel_pos.x, pixel_pos.y);
        if (!item)
                return;
        gbuffer->drawImageAt(*item->get_icon(), pixel_pos.x + 2, pixel_pos.y + 2);
        if (item->get_count() > 1)
        {
                gbuffer->setColour(juce::Colours::white);
                int yoff = cell_size - (int)gbuffer->getCurrentFont().getHeight();
                gbuffer->drawSingleLineText(String(item->get_count()), pixel_pos.x, pixel_pos.y + yoff);
        }
}

int Engine::get_width()
{
        return width;
}

int Engine::get_height()
{
        return height;
}

juce::Rectangle<int> Engine::get_camera_range()
{
        juce::Point<int> pos = map->get_player()->get_pos();
        return juce::Rectangle<int>(pos.x - width / 2, pos.y - height / 2, width, height);
}

Image* Engine::create_empty_image()
{
        return new Image(Image::RGB, width * cell_size, height * cell_size, true);
}

void Engine::render_obj(const game_object::GameObject* obj, const juce::Point<int> pos)
{
        if (!obj->get_sprite())
        {
                if (!map->get_player()->see(obj->get_pos()))
                        gbuffer->setColour(obj->get_color().darker());
                else
                        gbuffer->setColour(obj->get_color());
                juce::Rectangle<int> rect(cell_size, cell_size);
                rect.setPosition(pos * cell_size);
                gbuffer->fillRect(rect);
        }
        else
                render_obj_sprite(obj, pos);
}

void Engine::render_obj_sprite(const game_object::GameObject* obj, juce::Point<int> pos)
{
        const juce::Image* sprite = obj->get_sprite();
        int yoff = abs(cell_size - sprite->getHeight());
        int xoff = abs(cell_size - sprite->getWidth()) / 2;
        pos = (pos * cell_size) - juce::Point<int>(xoff, yoff);

        gbuffer->drawImageAt(*sprite, pos.x, pos.y);
        if (!map->get_player()->see(obj->get_pos()))
        {
                juce::Rectangle<int> r(pos.x, pos.y, sprite->getWidth(), sprite->getHeight());
                gbuffer->setColour(juce::Colour::fromRGBA(0, 0, 0, 128));
                gbuffer->fillRect(r);
                gbuffer->setColour(juce::Colours::black);
        }
        render_obj_state(obj, pos);
}

void Engine::render_sprite_centrified(const juce::Image* sprite, juce::Point<int> pos)
{
        pos *= cell_size;
        pos -= (juce::Point<int>(sprite->getWidth() - cell_size, sprite->getHeight() - cell_size) / 2);
        gbuffer->drawImageAt(*sprite, pos.x, pos.y);
}

void Engine::render_sprite_bottom_aligned(const juce::Image* sprite, juce::Point<int> pos)
{
        pos *= cell_size;
        pos.x -= (sprite->getWidth() - cell_size) / 2;
        pos.y += cell_size - sprite->getHeight();
        gbuffer->drawImageAt(*sprite, pos.x, pos.y);
}

void Engine::render_obj_state(const game_object::GameObject* obj, juce::Point<int> top_left)
{
        if (obj->get_stats().cur_hp == obj->get_stats().max_hp)
                return;
        top_left.y -= 3;
        const juce::Image* sprite = obj->get_sprite();
        juce::Rectangle<int> hp(top_left.x, top_left.y, sprite->getWidth(), 3);
        gbuffer->setColour(juce::Colours::black);
        gbuffer->fillRect(hp);
        gbuffer->setColour(juce::Colours::red);
        if (obj->get_pos() == obj->get_gdata()->get_map()->get_player()->get_pos())
                gbuffer->setColour(juce::Colours::yellowgreen);
        hp.setWidth((obj->get_stats().cur_hp * hp.getWidth()) / obj->get_stats().max_hp);
        gbuffer->fillRect(hp);
}

void Engine::render_submap(const juce::Rectangle<int> region)
{
        render_ground(region);
        render_other(region);
}

void Engine::render_hud()
{
        render_hud_left();
        render_hud_right();
}

void Engine::render_hud_left()
{
        const Stats& pstats = map->get_player()->get_stats();
        const juce::Image* orb = hud->get_health_orb((float)pstats.cur_hp / (float)pstats.max_hp);
        juce::Point<int> lpos(0, buffer->getHeight() - hud->get_left()->getHeight());
        gbuffer->drawImageAt(*orb, lpos.x, lpos.y);
        gbuffer->drawImageAt(*hud->get_left(), lpos.x, lpos.y);
}

void Engine::render_hud_right()
{
        const Stats& pstats = map->get_player()->get_stats();
        const juce::Image* orb = hud->get_mana_orb((float)pstats.cur_mana / (float)pstats.max_mana);
        juce::Point<int> rpos(buffer->getWidth() - hud->get_right()->getWidth(),
                buffer->getHeight() - hud->get_left()->getHeight());
        gbuffer->drawImageAt(*orb, rpos.x, rpos.y);
        gbuffer->drawImageAt(*hud->get_right(), rpos.x, rpos.y);
}

void Engine::render_obstacle(const game_object::StaticObject* obj, const juce::Point<int> pos)
{
        if (obj->hides_player())
                gbuffer->setColour(juce::Colour::fromRGBA(0, 0, 0, Settings::Graphics::get_wall_transparency()));
        render_obj(obj, pos);
        gbuffer->setColour(juce::Colours::black);
}

void Engine::render_ground(const juce::Rectangle<int> region)
{
        RectIterator<int> it = get_camera_range();
        while (!it.end())
        {
                juce::Point<int> pos = (*it++);
                if (map->in_range(pos) && map->get_tile(pos))
                {
                        GameMap::Tile* tile = map->get_tile(pos);
                        if (tile->static_obj && tile->static_obj->passable())
                                render_obj(tile->static_obj, pos - region.getTopLeft());
                }
        }
}

void Engine::render_other(const juce::Rectangle<int> region)
{
        RectIterator<int> it = get_camera_range();
        while (!it.end())
        {
                juce::Point<int> pos = *(it++);
                if (map->in_range(pos) && map->get_tile(pos))
                {
                        GameMap::Tile* tile = map->get_tile(pos);
                        bool visible = map->get_player()->see(pos);
                        pos -= region.getTopLeft();
                        if (tile->temp_objs.size() > 0)
                                render_temp_obj_list(tile->temp_objs, pos);

                        if (tile->dynamic_obj && visible)
                                render_obj(tile->dynamic_obj, pos);

                        if (tile->animations->size() > 0)
                                render_animation_list(tile->animations, pos);

                        if (tile->static_obj && !tile->static_obj->passable())
                                render_obstacle(tile->static_obj, pos);
                }
        }
}

void Engine::render_temp_obj_list(std::list<game_object::TempObject*> list, const juce::Point<int> pos)
{
        std::list<game_object::TempObject*>::iterator it = list.begin();
        while (it != list.end())
                render_sprite_centrified((*it++)->get_sprite(), pos);
}

void Engine::render_animation_list(AnimationList* list, const juce::Point<int> pos)
{
        size_t size = list->size();
        for (size_t i = 0; i < size; i++)
        {
                Animation* animation = list->pop();
                if (animation->is_done())
                        delete animation;
                else
                {
                        list->push(animation);
                        if (animation->get_align() == Animation::Align::center)
                                render_sprite_centrified(animation->get_frame(), pos);
                        else
                                render_sprite_bottom_aligned(animation->get_frame(), pos);
                }
        }
}
