#include "StaticObject.h"
#include "DynamicObject.h"
#include "GlobalData.h"
#include "Resource.h"

game_object::Ground::Ground(juce::Point<int> pos)
        : StaticObject(pos, juce::Colours::green)
{
        sprite = Resource::get_ground_tile();
}

void game_object::Ground::collide(DynamicObject* target)
{
        GameMap& map = *this->gdata->get_map();
        if (map.has_dyn_obj(this->pos))
                return;
        map.get_tile(target->get_pos())->dynamic_obj = nullptr;
        target->set_pos(this->pos);
        map.get_tile(this->get_pos())->dynamic_obj = target;
}

void game_object::Ground::collide(Projectile* target)
{
        collide(static_cast<DynamicObject*>(target));
}

game_object::Wall::Wall(juce::Point<int> pos)
        : StaticObject(pos, juce::Colours::darkgrey)
{
        height = 3;
        sprite = Resource::get_wall_sprite();
}

int game_object::StaticObject::get_height() const
{
        return height;
}

bool game_object::StaticObject::hides_player() const
{
        juce::Point<int> player_pos = gdata->get_map()->get_player()->get_pos();
        if (abs(pos.x - player_pos.x) > 1)
                return false;
        int delta = pos.y - player_pos.y;
        return delta <= height && delta >= 0;
}

void game_object::StaticObject::on_death()
{
        // animation
        Ground* floor = new Ground(pos);
        floor->set_gdata(gdata);
        gdata->get_map()->get_tile(pos)->static_obj = floor;
        GameObject::on_death();
}

const juce::Image* game_object::StaticObject::get_sprite() const
{
        return sprite;
}

void game_object::StaticObject::collide(Projectile* target)
{
        target->on_death();
        if (breakable())
                this->take_damage(target->get_stats().damage);
}
