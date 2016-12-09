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
        Graphics g(*buffer);
        g.fillAll(Colours::black);
        render_submap(g, get_camera_range());
        render_hud(g);
        render_hotkey_tab(g);
        return buffer;
}

const Image* Engine::render_inventory()
{
        render();
        Graphics g(*buffer);
        g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 180));
        render_backpack(g);
        return buffer;
}

const Image* Engine::render_gameover()
{
        Graphics g(*buffer);
        g.fillAll(Colours::black);
        g.setColour(Colours::white);
        const juce::String msg("GAME OVER, press ENTER to restart...");
        g.drawSingleLineText(msg,
                (buffer->getWidth() - g.getCurrentFont().getStringWidth(msg)) / 2, buffer->getHeight() / 2);
        return buffer;
}

void Engine::render_hotkey_tab(juce::Graphics& g)
{
        int tile_size = Resource::get_inventory_tile()->getWidth() + 1;
        const PlayerInventory* inv = map->get_player()->get_inventory();
        juce::Point<int> off((buffer->getWidth() - cell_size * inv->get_width()) / 2,
                buffer->getHeight() - 100);
        for (int i = 0; i < inv->get_width(); i++)
        {
                juce::Point<int> pos(i, 0);
                render_item(g, inv->get_item(pos), off + pos * tile_size,
                        map->get_player()->get_current_item() == i, false);
        }
}

void Engine::render_backpack(juce::Graphics& g)
{
        int tile_size = Resource::get_inventory_tile()->getWidth() + 1;
        const PlayerInventory* inv = map->get_player()->get_inventory();
        juce::Point<int> off((buffer->getWidth() - tile_size * inv->get_width()) / 2,
                (buffer->getHeight() - tile_size * inv->get_height()) / 2);
        for (int i = 0; i < inv->get_height(); i++)
                for (int j = 0; j < inv->get_width(); j++)
                {
                        juce::Point<int> pos(j, i);
                        render_item(g, inv->get_item(pos),
                                off + pos * tile_size,
                                pos == inv->get_selection_tile(),
                                inv->is_selected());
                }
}

void Engine::render_item(juce::Graphics& g, const Item* item, juce::Point<int> pixel_pos, bool selection_tile, bool selected)
{
        const Image* tile = Resource::get_inventory_tile();
        if (selection_tile)
                tile = selected ? Resource::get_inventory_selected_tile()
                        : Resource::get_inventory_tile_selection();
        g.drawImageAt(*tile, pixel_pos.x, pixel_pos.y);
        if (!item)
                return;
        g.drawImageAt(*item->get_icon(), pixel_pos.x + 2, pixel_pos.y + 2);
        if (item->get_count() > 1)
        {
                g.setColour(juce::Colours::white);
                int yoff = cell_size - (int)g.getCurrentFont().getHeight();
                g.drawSingleLineText(String(item->get_count()), pixel_pos.x, pixel_pos.y + yoff);
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

void Engine::render_obj(juce::Graphics& g, const game_object::GameObject* obj, const juce::Point<int> pos)
{
        if (!obj->get_sprite())
        {
                if (!map->get_player()->see(obj->get_pos()))
                        g.setColour(obj->get_color().darker());
                else
                        g.setColour(obj->get_color());
                juce::Rectangle<int> rect(cell_size, cell_size);
                rect.setPosition(pos * cell_size);
                g.fillRect(rect);
        }
        else
                render_obj_sprite(g, obj, pos);
}

void Engine::render_obj_sprite(juce::Graphics& g, const game_object::GameObject* obj, juce::Point<int> pos)
{
        const juce::Image* sprite = obj->get_sprite();
        int yoff = abs(cell_size - sprite->getHeight());
        int xoff = abs(cell_size - sprite->getWidth()) / 2;
        pos = (pos * cell_size) - juce::Point<int>(xoff, yoff);

        g.drawImageAt(*sprite, pos.x, pos.y);
        if (!map->get_player()->see(obj->get_pos()))
        {
                juce::Rectangle<int> r(pos.x, pos.y, sprite->getWidth(), sprite->getHeight());
                g.setColour(juce::Colour::fromRGBA(0, 0, 0, 128));
                g.fillRect(r);
                g.setColour(juce::Colours::black);
        }
        render_obj_state(g, obj, pos);
}

void Engine::render_sprite_centrified(juce::Graphics& g, const juce::Image* sprite, juce::Point<int> pos)
{
        pos *= cell_size;
        pos -= (juce::Point<int>(sprite->getWidth() - cell_size, sprite->getHeight() - cell_size) / 2);
        g.drawImageAt(*sprite, pos.x, pos.y);
}

void Engine::render_sprite_bottom_aligned(juce::Graphics& g, const juce::Image* sprite, juce::Point<int> pos)
{
        pos *= cell_size;
        pos.x -= (sprite->getWidth() - cell_size) / 2;
        pos.y += cell_size - sprite->getHeight();
        g.drawImageAt(*sprite, pos.x, pos.y);
}

void Engine::render_obj_state(juce::Graphics& g, const game_object::GameObject* obj, juce::Point<int> top_left)
{
        if (obj->get_stats().cur_hp == obj->get_stats().max_hp)
                return;
        top_left.y -= 3;
        const juce::Image* sprite = obj->get_sprite();
        juce::Rectangle<int> hp(top_left.x, top_left.y, sprite->getWidth(), 3);
        g.setColour(juce::Colours::black);
        g.fillRect(hp);
        g.setColour(juce::Colours::red);
        if (obj->get_pos() == obj->get_gdata()->get_map()->get_player()->get_pos())
                g.setColour(juce::Colours::yellowgreen);
        hp.setWidth((obj->get_stats().cur_hp * hp.getWidth()) / obj->get_stats().max_hp);
        g.fillRect(hp);
}

void Engine::render_submap(juce::Graphics& g, const juce::Rectangle<int> region)
{
        render_ground(g, region);
        render_other(g, region);
}

void Engine::render_hud(juce::Graphics& g)
{
        render_hud_left(g);
        render_hud_right(g);
}

void Engine::render_hud_left(juce::Graphics& g)
{
        const Stats& pstats = map->get_player()->get_stats();
        const juce::Image* orb = hud->get_health_orb((float)pstats.cur_hp / (float)pstats.max_hp);
        juce::Point<int> lpos(0, buffer->getHeight() - hud->get_left()->getHeight());
        g.drawImageAt(*orb, lpos.x, lpos.y);
        g.drawImageAt(*hud->get_left(), lpos.x, lpos.y);
}

void Engine::render_hud_right(juce::Graphics& g)
{
        const Stats& pstats = map->get_player()->get_stats();
        const juce::Image* orb = hud->get_mana_orb((float)pstats.cur_mana / (float)pstats.max_mana);
        juce::Point<int> rpos(buffer->getWidth() - hud->get_right()->getWidth(),
                buffer->getHeight() - hud->get_left()->getHeight());
        g.drawImageAt(*orb, rpos.x, rpos.y);
        g.drawImageAt(*hud->get_right(), rpos.x, rpos.y);
}

void Engine::render_obstacle(juce::Graphics& g, const game_object::StaticObject* obj, const juce::Point<int> pos)
{
        if (obj->hides_player())
                g.setColour(juce::Colour::fromRGBA(0, 0, 0, Settings::Graphics::get_wall_transparency()));
        render_obj(g, obj, pos);
        g.setColour(juce::Colours::black);
}

void Engine::render_ground(juce::Graphics& g, const juce::Rectangle<int> region)
{
        RectIterator<int> it = get_camera_range();
        while (!it.end())
        {
                juce::Point<int> pos = (*it++);
                if (map->in_range(pos) && map->get_tile(pos))
                {
                        GameMap::Tile* tile = map->get_tile(pos);
                        if (tile->static_obj && tile->static_obj->passable())
                                render_obj(g, tile->static_obj, pos - region.getTopLeft());
                }
        }
}

void Engine::render_other(juce::Graphics& g, const juce::Rectangle<int> region)
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
                                render_temp_obj_list(g, tile->temp_objs, pos);

                        if (tile->dynamic_obj && visible)
                                render_obj(g, tile->dynamic_obj, pos);

                        if (tile->animations->size() > 0)
                                render_animation_list(g, tile->animations, pos);

                        if (tile->static_obj && !tile->static_obj->passable())
                                render_obstacle(g, tile->static_obj, pos);
                }
        }
}

void Engine::render_temp_obj_list(juce::Graphics& g, std::list<game_object::TempObject*> list, const juce::Point<int> pos)
{
        std::list<game_object::TempObject*>::iterator it = list.begin();
        while (it != list.end())
                render_sprite_centrified(g, (*it++)->get_sprite(), pos);
}

void Engine::render_animation_list(juce::Graphics& g, AnimationList* list, const juce::Point<int> pos)
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
                                render_sprite_centrified(g ,animation->get_frame(), pos);
                        else
                                render_sprite_bottom_aligned(g, animation->get_frame(), pos);
                }
        }
}
