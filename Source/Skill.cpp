#include "Skill.h"
#include "Resource.h"
#include "DynamicObject.h"
#include "GlobalData.h"
#include "Engine.h"
#include "Random.h"

Skill::Skill(const Frames* frames)
        : frames(frames), mana_cost(0), hp_cost(0)
{
}

void Skill::use(game_object::Character* user)
{
}

void Skill::add_animation(game_object::Character* user)
{
        user->get_gdata()->get_map()->add_animation(new OneTimeAnimation(frames), user->get_pos());
}

void Skill::deal_damage(game_object::Character* user, juce::Point<int> pos, int dmg)
{
        GameMap& map = *user->get_gdata()->get_map();
        if (map.has_static_obj(pos) && map.get_static_obj(pos)->breakable())
                map.get_static_obj(pos)->take_damage(dmg);
        if (map.has_dyn_obj(pos))
                map.get_dyn_obj(pos)->take_damage(dmg);
}

bool Skill::enough_stats(game_object::Character* user)
{
        return user->get_stats().cur_mana >= mana_cost
                && user->get_stats().cur_hp >= hp_cost;
}

void Skill::burn_stats(game_object::Character* user)
{
        Stats new_stats = user->get_stats();
        new_stats.cur_mana -= mana_cost;
        new_stats.cur_hp -= hp_cost;
        user->set_stats(new_stats);
}

BladeVortex::BladeVortex()
        : Skill(Resource::get_bladevortex_frames())
{
}

void BladeVortex::use(game_object::Character* user)
{
        if (!enough_stats(user))
                return;
        burn_stats(user);
        add_animation(user);
        for (int i = 0; i < 4; i++)   
                deal_damage(user, user->get_pos() + directions[i], user->get_stats().get_dmg());
}

FireballSkill::FireballSkill()
        : Skill(nullptr)
{
}

void FireballSkill::create_projectile(game_object::Character* user, game_object::Projectile* p)
{
        p->set_damage(user->get_stats());
        p->set_gdata(user->get_gdata());
        GameMap& map = *user->get_gdata()->get_map();
        juce::Point<int> pos = p->get_pos();
        if (!map.free_to_move(pos))
        {
                user->get_gdata()->get_map()->add_animation(p->get_explosion_animation(),
                        map.in_range(pos) ? pos : user->get_pos());
                deal_damage(user, pos, p->get_dmg());
                delete p;
                return;
        }
        user->get_gdata()->get_map()->get_tile(pos)->dynamic_obj = p;
}

void FireballSkill::create_fireball(game_object::Character* user, juce::Point<int> dir)
{
        create_projectile(user, new game_object::Fireball(user->get_pos() + dir, dir));
}

PlayerFireball::PlayerFireball()
        : FireballSkill()
{
        mana_cost = 10;
}

void PlayerFireball::use(game_object::Character* user)
{
        if (!enough_stats(user))
                return;
        burn_stats(user);
        juce::Point<int> dir = directions[user->get_direction()];
        create_fireball(user, dir);
}

MonsterFireball::MonsterFireball()
        : FireballSkill()
{
}

void MonsterFireball::use(game_object::Character* user)
{
        juce::Point<int> v = user->get_gdata()->get_map()->get_player()->get_pos() - user->get_pos();
        if (v.x != 0 && v.y != 0 && v.x != v.y)
                return;
        juce::Point<int> dir = v;
        if (dir.x < 0 || dir.x > 0)
                dir.x = dir.x < 0 ? -1 : 1;
        if (dir.y < 0 || dir.y > 0)
                dir.y = dir.y < 0 ? -1 : 1;
        create_fireball(user, dir);
}

DarkFireball::DarkFireball()
        : FireballSkill()
{
}

void DarkFireball::use(game_object::Character* user)
{
        for (int i = 0; i < 8; i++)
                create_projectile(user, new game_object::DarkFireball(user->get_pos() + directions[i], directions[i]));
}

PlayerDarkFireball::PlayerDarkFireball()
        : DarkFireball()
{
        mana_cost = 12;
}

void PlayerDarkFireball::use(game_object::Character* user)
{
        if (!enough_stats(user))
                return;
        burn_stats(user);
        DarkFireball::use(user);
}

FlickerStrike::FlickerStrike()
        :Skill(Resource::get_flickerstrike_frames())
{
}

void FlickerStrike::use(game_object::Character* user, game_object::DynamicObject* target)
{
        GameMap& map = *user->get_gdata()->get_map();
        juce::Point<int> pos;
        for (int i = 0; i < 4; i++)
        {
                pos = target->get_pos() + directions[i];
                if (map.free_to_move(pos))
                {
                        add_animation(user);
                        user->move(pos);
                        deal_damage(user, target->get_pos(), user->get_stats().get_dmg() * 1.8);
                        return;
                }
        }
}

PlayerFlickerStrike::PlayerFlickerStrike()
        :FlickerStrike()
{
        mana_cost = 3;
        hp_cost = 1;
}

void PlayerFlickerStrike::use(game_object::Character* user)
{
        game_object::DynamicObject* obj = get_random_enemy(user);
        if (obj)
        {
                if (!enough_stats(user))
                        return;
                burn_stats(user);
                FlickerStrike::use(user, obj);
        }
}

game_object::DynamicObject* PlayerFlickerStrike::get_random_enemy(game_object::Character* user)
{
        GameMap& map = *user->get_gdata()->get_map();
        std::list<game_object::DynamicObject*> obj_list = map.get_visible_dyn_obj_list();
        if (obj_list.size() == 1) // only player
                return nullptr; 
        std::list<game_object::DynamicObject*>::iterator it;
        while (1)
        {
                unsigned rnd = (unsigned)random.nextInt() % obj_list.size();
                it = obj_list.begin();
                for (unsigned i = 0; i < rnd; i++)
                        it++;
                if ((*it)->get_pos() != user->get_pos())
                        return (*it);
        }
}

MonsterFlickerStrike::MonsterFlickerStrike()
        :FlickerStrike()
{
}

void MonsterFlickerStrike::use(game_object::Character* user)
{
        game_object::Character* player = user->get_gdata()->get_map()->get_player();
        if (user->see(player->get_pos()))
                FlickerStrike::use(user, player);
}

Darkfire::Darkfire()
        :Skill(Resource::get_darkfire_frames())
{
}

void Darkfire::use(game_object::Character* user)
{
        int dmg = user->get_stats().cur_hp * 1.1;
        mana_cost = user->get_stats().cur_mana - 1;
        hp_cost = user->get_stats().cur_hp - 1;
        burn_stats(user);
        for (int i = 0; i < 4; i++)
        {
                juce::Point<int> pos = user->get_pos() + directions[i];
                deal_damage(user, pos, dmg);
                add_animation(user);
        }
}

FirestormSkill::FirestormSkill()
        : Skill(nullptr)
{
}

void FirestormSkill::use(game_object::Character* user)
{
        std::vector<juce::Point<int>> positions = get_random_positions_in_vision(user, 6);
        for (int i = 0; i < positions.size(); i++)
                create_firestorm(user, positions[i]);
}

void FirestormSkill::create_firestorm(game_object::Character* user, juce::Point<int> pos, int latency)
{
        game_object::Firestorm* firestorm = new game_object::Firestorm(pos, latency);
        firestorm->set_gdata(user->get_gdata());
        firestorm->set_dmg(user->get_stats());
        user->get_gdata()->get_map()->add_temp_obj(firestorm);
}

std::vector<juce::Point<int>> FirestormSkill::get_random_positions_in_vision(game_object::Character* user, int count) const
{
        std::vector<juce::Point<int>> result(count);
        GameMap& map = *user->get_gdata()->get_map();
        juce::Rectangle<int> range = Engine::get_camera_range();
        for (int i = 0; i < count; i++)
        {
                juce::Point<int> pos = range.getTopLeft();;
                pos.x += RNG::next_uint() % range.getWidth();
                pos.y += RNG::next_uint() % range.getHeight();
                while (!(map.has_static_obj(pos) && map.get_static_obj(pos)->passable() && user->distance(pos) <= 1.4
                        && pos != user->get_pos()))
                {
                        pos = range.getTopLeft();
                        pos.x += RNG::next_uint() % range.getWidth();
                        pos.y += RNG::next_uint() % range.getHeight();
                }
                result[i] = pos;
        }
        return result;
}

PlayerFirestorm::PlayerFirestorm()
        : FirestormSkill()
{
        mana_cost = 14;
}

void PlayerFirestorm::use(game_object::Character* user)
{
        if (!enough_stats(user))
                return;
        burn_stats(user);
        FirestormSkill::use(user);
}

DragonFirestorm::DragonFirestorm()
        : FirestormSkill()
{
}

void DragonFirestorm::use(game_object::Character* user)
{
        GameMap& map = *user->get_gdata()->get_map();
        RectIterator<int> it = Engine::get_camera_range();
        while (!it.end())
        {
                juce::Point<int> pos = *(it++);
                if (!(map.has_static_obj(pos) && map.get_static_obj(pos)->passable() && user->see(pos)
                                && pos != user->get_pos()))
                        continue;
                create_firestorm(user, pos, user->distance(pos));
        }
}
