#include "TempObject.h"
#include "GlobalData.h"
#include "Map.h"
#include "Resource.h"

game_object::TempObject::TempObject(juce::Point<int> pos, unsigned lifetime)
        :GameObject(static_object_stats, pos, juce::Colours::yellow), lifetime(lifetime)
{
}

void game_object::TempObject::move()
{
        if (--lifetime == 0)
                on_death();
}

void game_object::TempObject::on_death()
{
        delete_from_map();
        delete this;
}

void game_object::TempObject::delete_from_map()
{
        GameMap::Tile* tile = gdata->get_map()->get_tile(pos);
        std::list<game_object::TempObject*>::iterator it = tile->temp_objs.begin();
        while (it != tile->temp_objs.end())
        {
                if (*it == this)
                {
                        tile->temp_objs.erase(it);
                        return;
                }
                it++;
        }
}

game_object::Drop::Drop(juce::Point<int> pos, Item* item)
        : TempObject(pos), item(item)
{
}

void game_object::Drop::collide(Character* target)
{
        target->collide(this);
}

const juce::Image* game_object::Drop::get_sprite() const
{
        return item->get_icon();
}

Item* game_object::Drop::get_item() const
{
        return item;
}

game_object::Portal::Portal(juce::Point<int> pos)
        : TempObject(pos), dest(nullptr)
{
        anim = new Animation(Resource::get_portal_frames(), 8);
}

void game_object::Portal::collide(Character* target)
{
        if (!dest)
                return;
        GameMap* map = target->get_gdata()->get_map();
        juce::Point<int> new_pos;
        for (int i = 0; i < 4; i++)
        {
                new_pos = dest->get_pos() + directions[i];
                if (map->free_to_move(new_pos))
                {
                        target->move(new_pos);
                        return;
                }
        }
}

const juce::Image* game_object::Portal::get_sprite() const
{
        return anim->get_frame();
}

void game_object::Portal::bind(Portal* dest)
{
        this->dest = dest;
        dest->dest = this;
}

game_object::Firestorm::Firestorm(juce::Point<int> pos, int latency)
        : TempObject(pos, 1), dmg(5)
{
        this->lifetime = latency;
}

const juce::Image* game_object::Firestorm::get_sprite() const
{
        return Resource::get_empty_sprite();
}

void game_object::Firestorm::set_dmg(Stats user)
{
        dmg = user.get_dmg() * 1.45;
}

void game_object::Firestorm::on_death()
{
        GameMap& map = *this->gdata->get_map();
        map.add_animation(new OneTimeAnimation(Resource::get_firestorm_frames(), 25U, true, Animation::Align::bottom), pos);
        if (map.has_dyn_obj(pos))
                map.get_dyn_obj(pos)->take_damage(dmg);
        TempObject::on_death();
}
