#include "DynamicObject.h"
#include "StaticObject.h"
#include "TempObject.h"
#include "GlobalData.h"
#include "Resource.h"
#include "Drop.h"

void game_object::Character::collide(Projectile* target)
{
        take_damage(target->get_stats().damage);
        target->on_death();
}

const juce::Image* game_object::Character::get_sprite() const
{
        return this->cur_frame;
}

void game_object::Character::attack()
{
}

void game_object::Character::drop_item(Item* item)
{
        Drop* drop = new game_object::Drop(pos, item);
        drop->set_gdata(gdata);
        gdata->get_map()->add_temp_obj(drop);
}

int game_object::Character::get_direction() const
{
        return direction;
}

bool game_object::Character::see(juce::Point<int> pos)
{
        return see(pos, stats.vision);
}

bool game_object::Character::see(juce::Point<int> pos, int r)
{
        if (distance(pos) > (double)r)
                return false;
        int length = distance(pos);
        juce::Point<int> delta(pos.x - this->pos.x, pos.y - this->pos.y);
        for (int i = 1; i <= length; i++)
        {
                juce::Point<int> p = this->pos + (delta * i) / length;
                if (p == this->pos)
                        continue;
                if (!gdata->get_map()->in_range(p))
                        return false;
                const GameMap::Tile* cell = gdata->get_map()->get_tile(p);
                if (cell->static_obj && !cell->static_obj->passable() || cell->dynamic_obj)
                        return i == length;
        }
        return true;
}

bool game_object::Character::in_vision(juce::Point<int> pos)
{
        return distance(pos) <= (double)stats.vision;
}

void game_object::Character::move(juce::Point<int> next_pos)
{
        GameObject::move();
        GameMap& map = *gdata->get_map();
        if (!map.in_range(next_pos))
                return;
        set_frame_from_next_pos(next_pos);
        if (map.has_dyn_obj(next_pos))
                map.get_dyn_obj(next_pos)->collide(this);
        if (map.has_static_obj(next_pos))
                map.get_static_obj(next_pos)->collide(this);
        collide_with_temp_objs(next_pos);
}

void game_object::Character::collide_with_temp_objs(juce::Point<int> next_pos)
{
        GameMap::Tile* tile = gdata->get_map()->get_tile(next_pos);
        std::list<game_object::TempObject*>::iterator it = tile->temp_objs.begin();
        while (it != tile->temp_objs.end())
                (*it++)->collide(this);
}

void game_object::Character::set_frame_from_next_pos(juce::Point<int> next_pos)
{
        if (!sprite || pos == next_pos)
                return;
        juce::Point<int> delta = next_pos - pos;
        if (delta.x > 0)
        {
                cur_frame = sprite->get_right();
                direction = dir_right;
        }
        else if (delta.x < 0)
        {
                cur_frame = sprite->get_left();
                direction = dir_left;
        }
        else if (delta.y > 0)
        {
                cur_frame = sprite->get_front();
                direction = dir_down;
        }
        else
        {
                cur_frame = sprite->get_rear();
                direction = dir_up;
        }
}

void game_object::Character::set_sprite(const CharacterSprite* sprite)
{
        this->sprite = sprite;
        cur_frame = sprite->get_front();
}

game_object::Character::Character(juce::Point<int> pos, juce::Colour color)
        : DynamicObject(pos, color), ai(nullptr), cur_frame(nullptr), direction(dir_down)
{
}

game_object::Character::Character(const Character& obj)
        : DynamicObject(*this)
{
        this->direction = obj.direction;
        this->ai = obj.ai;
        this->sprite = obj.sprite;
        this->cur_frame = obj.cur_frame;
}

void game_object::Character::move()
{
        Character::move(ai->next_move());
}

void game_object::Character::take_damage(int dmg)
{
        gdata->get_map()->add_animation(new OneTimeAnimation(Resource::get_blood_frames()), pos);
        GameObject::take_damage(dmg);
}

game_object::Knight::Knight(juce::Point<int> pos)
        : Character(pos, juce::Colours::aqua), current_item_idx(0)
{
        stats = player_stats;
        set_sprite(Resource::get_player_sprite());
        inventory = new PlayerInventory();

        inventory->add_item(new HurricaneSword());
        inventory->add_item(new SmallHealPotion());
        inventory->add_item(new SmallHealPotion());
        inventory->add_item(new SmallHealPotion());
        inventory->add_item(new SmallHealPotion());


        //inventory->add_item(new FirestormScroll());
        //inventory->add_item(new BlinkDagger());
        ////inventory->add_item(new DarkfireScroll());
        //inventory->add_item(new FirestormScroll());
        //inventory->add_item(new DarkPyromancyFlame());
}

void game_object::Knight::on_death()
{
        this->gdata->set_game_over(true);
}

void game_object::Knight::collide(Drop* target)
{
        inventory->add_item(target->get_item());
        target->on_death();
}

void game_object::Knight::collide(Princess* target)
{
        this->gdata->set_game_over(true);
}

void game_object::Knight::move()
{
        if (pos != next_move)
                Character::move(next_move);
}

void game_object::Knight::attack()
{
        if (inventory->get_item(juce::Point<int>(current_item_idx, 0)))
        {
                inventory->get_item(juce::Point<int>(current_item_idx, 0))->use(this);
                inventory->update();
                this->next_move = pos;
        }
}

PlayerInventory* game_object::Knight::get_inventory() const
{
        return inventory;
}

int game_object::Knight::get_current_item() const
{
        return current_item_idx;
}

void game_object::Knight::set_current_item(int idx)
{
        current_item_idx = idx;
}

void game_object::Knight::move_left()
{
        this->next_move = pos + directions[dir_left];
}

void game_object::Knight::move_right()
{
        this->next_move = pos + directions[dir_right];
}

void game_object::Knight::move_top()
{
        this->next_move = pos + directions[dir_up];
}

void game_object::Knight::move_bottom()
{
        this->next_move = pos + directions[dir_down];
}

game_object::Zombie::Zombie(juce::Point<int> pos) : Monster(pos, juce::Colours::brown)
{
        ai = new ZombieAI(this);
        stats = zombie_stats;
        set_sprite(Resource::get_zombie_sprite());
}

void game_object::DynamicObject::on_death()
{
        gdata->get_map()->get_tile(pos)->dynamic_obj = nullptr;
        GameObject::on_death();
}

void game_object::Monster::on_death()
{
        drop_loot();
        DynamicObject::on_death();
}

void game_object::Monster::attack()
{
        Knight* player = gdata->get_map()->get_player();
        for (int i = 0; i < 4; i++)
        {
                juce::Point<int> pos = this->pos + directions[i];
                if (pos == player->get_pos())
                        player->take_damage(this->stats.damage);
        }
}

void game_object::Monster::drop_loot()
{
        std::vector<Item*> loot = DropManager::get_drop(iir, iiq);
        for (int i = 0; i < loot.size(); i++)
                drop_item(loot[i]);
}

game_object::Demon::Demon(juce::Point<int> pos)
        : Monster(pos, juce::Colours::orangered)
{
        iir = 250;
        iiq = 1000;
        ai = new DemonAI(this);
        stats = demon_stats;
        set_sprite(Resource::get_demon_sprite());
        skill = new MonsterFireball();
}

void game_object::Demon::attack()
{
        Knight* player = gdata->get_map()->get_player();
        if (!see(player->get_pos()))
                return;
        if (distance(player->get_pos()) > stats.vision / 2)
                skill->use(this);
        else
                Monster::attack();
}

void game_object::Demon::drop_loot()
{
        drop_item(new MediumHealPotion());
        Monster::drop_loot();
}

game_object::Projectile::Projectile(juce::Point<int> pos, juce::Point<int> dir)
        : DynamicObject(pos, juce::Colours::aliceblue), dir(dir), dmg_mult(1.0)
{
        animation = nullptr;
}

game_object::Projectile::~Projectile()
{
}

void game_object::Projectile::collide(Character* target)
{
        target->take_damage(stats.damage);
        on_death();
}

void game_object::Projectile::collide(Projectile* target)
{
        this->on_death();
        target->on_death();
}

const juce::Image* game_object::Projectile::get_sprite() const
{
        return animation->get_frame();
}

int game_object::Projectile::get_dmg() const
{
        return stats.get_dmg() * dmg_mult;
}

double game_object::Projectile::get_dmg_mult() const
{
        return dmg_mult;
}

void game_object::Projectile::set_damage(Stats user_stats)
{
        stats.damage = user_stats.damage * dmg_mult;
}

void game_object::Projectile::move()
{
        juce::Point<int> next_pos = pos + dir;
        int length = distance(next_pos);
        juce::Point<int> delta(next_pos.x - this->pos.x, next_pos.y - this->pos.y);
        for (int i = 0; i <= length; i++)
        {
                juce::Point<int> p = this->pos +(delta * i) / length;
                if (p == this->pos)
                        continue;
                if (!gdata->get_map()->in_range(p))
                {
                        on_death();
                        return;
                }
                GameMap::Tile* t = gdata->get_map()->get_tile(p);
                if (t->static_obj && !t->static_obj->passable())
                {
                        t->static_obj->collide(this);
                        return;
                }
                if (t->dynamic_obj)
                {
                        t->dynamic_obj->collide(this);
                        return;
                }
        }
        gdata->get_map()->get_static_obj(next_pos)->collide(this);
}

void game_object::Projectile::on_death()
{
        gdata->get_map()->add_animation(get_explosion_animation(), pos);
        DynamicObject::on_death();
}

game_object::Fireball::Fireball(juce::Point<int> pos, juce::Point<int> dir)
        : Projectile(pos, dir)
{
        animation = new Animation(Resource::get_fireball_frames(), 16);
        dmg_mult = 2.3;
}

Animation* game_object::Fireball::get_explosion_animation() const
{
        return new OneTimeAnimation(Resource::get_fireball_explosion_frames(), 60);
}

game_object::DarkFireball::DarkFireball(juce::Point<int> pos, juce::Point<int> dir)
        : Projectile(pos, dir)
{
        animation = new Animation(Resource::get_dark_fireball_frames(), 16);
        dmg_mult = 2.1;
}

Animation* game_object::DarkFireball::get_explosion_animation() const
{
        return new OneTimeAnimation(Resource::get_dark_fireball_explosion_frames(), 160);
}

game_object::Princess::Princess(juce::Point<int> pos)
        : Character(pos, juce::Colours::aquamarine)
{
        ai = new NoneAI(this);
        set_sprite(Resource::get_princess_sprite());
}

game_object::Skeleton::Skeleton(juce::Point<int> pos)
        : Monster(pos)
{
        iir = 600;
        iiq = 350;
        ai = new SkeletonAI(this);
        set_stats(skeleton_stats);
        skill = new MonsterFlickerStrike();
        set_sprite(Resource::get_skeleton_sprite());
}

void game_object::Skeleton::attack()
{
        Knight* player = gdata->get_map()->get_player();
        if (!see(player->get_pos()))
                return;
        if (distance(player->get_pos()) > 1.5)
                skill->use(this);
        else
                Monster::attack();
}

game_object::Dragon::Dragon(juce::Point<int> pos)
        : Monster(pos)
{
        iir = 800;
        iiq = 800;
        ai = new DragonAI(this);
        set_stats(dragon_stats);
        firestorm = new DragonFirestorm();
        set_sprite(Resource::get_dragon_sprite());

        firestorm_used = false;
}

void game_object::Dragon::attack()
{
        Knight* player = gdata->get_map()->get_player();
        if (!see(player->get_pos()))
                return;
        if (distance(player->get_pos()) <= stats.vision / 2 && RNG::next_uint() % 10 == 0)
        {
                firestorm->use(this);
                firestorm_used = true;
        }
        else
                Monster::attack();
}

void game_object::Dragon::move()
{
        if (firestorm_used)
        {
                firestorm_used = false;
                return;
        }
        Character::move();
}
