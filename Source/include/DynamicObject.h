#pragma once

#include "AI.h"
#include "GameObject.h"
#include "CharacterSprite.h"
#include "Skill.h"
#include "Inventory.h"

namespace game_object
{

class DynamicObject : public GameObject
{
public:
        DynamicObject(juce::Point<int> pos, juce::Colour color = juce::Colours::blue)
                : GameObject(dynamic_object_stats, pos, color)
        {
        }
        DynamicObject(const DynamicObject& obj)
                : GameObject(*this)
        {
        }

        virtual void on_death() override;
        virtual void collide(GameObject* target) override
        {
                target->collide(this);
        }
        virtual void collide(StaticObject* target) override
        {
        }
        virtual void collide(DynamicObject* target) override
        {
        }
        virtual void collide(Character* target)
        {
        }
        virtual void collide(Projectile* target)
        {
        }
};

class Character : public DynamicObject
{
public:
        Character(juce::Point<int> pos, juce::Colour color = juce::Colours::blueviolet);
        Character(const Character& obj);

        virtual void move() override;
        virtual void take_damage(int dmg) override;
        virtual void collide(Character* target) override
        {
        }
        virtual void collide(Projectile* target) override;
        virtual void collide(Princess* target)
        {
        }
        virtual void collide(Drop* target)
        {
        }
        virtual const juce::Image* get_sprite() const override;
        virtual void attack();

        void drop_item(Item* item);
        void move(juce::Point<int> next_pos);
        int get_direction() const;
        bool see(juce::Point<int> pos);
        bool see(juce::Point<int> pos, int r);
        bool in_vision(juce::Point<int> pos);
protected:
        void collide_with_temp_objs(juce::Point<int> next_pos);
        void set_frame_from_next_pos(juce::Point<int> next_pos);
        void set_sprite(const CharacterSprite* sprite);

        int direction;
        CharacterAI* ai;
        const CharacterSprite* sprite;
        const juce::Image* cur_frame;
};

class Knight : public Character
{
public:
        Knight(juce::Point<int> pos);

        virtual void on_death() override;
        virtual void collide(Drop* target) override;
        virtual void collide(Princess* target) override;
        virtual void move() override;
        virtual void attack() override;

        PlayerInventory* get_inventory() const;
        int get_current_item() const;
        void set_current_item(int idx);
        void move_left();
        void move_right();
        void move_top();
        void move_bottom();
protected:
        PlayerInventory* inventory;
        juce::Point<int> next_move;
        int current_item_idx;
};

class Princess : public Character
{
public:
        Princess(juce::Point<int> pos);
        virtual void collide(Character* target) override
        {
                target->collide(this);
        }
};

class Monster : public Character
{
public:
        Monster(juce::Point<int> pos, juce::Colour color = juce::Colours::red)
                : Character(pos, color), skill(nullptr), iir(10), iiq(130) { }

        virtual void on_death() override;
        virtual void attack() override;
protected:
        Skill* skill;
        virtual void drop_loot();

        int iir;
        int iiq;
};

class Zombie : public Monster
{
public:
        Zombie(juce::Point<int> pos);
};

class Skeleton : public Monster
{
public:
        Skeleton(juce::Point<int> pos);
        virtual void attack() override;
};

class Demon : public Monster
{
public:
        Demon(juce::Point<int> pos);
        virtual void attack() override;
protected:
        virtual void drop_loot();
};

class Dragon : public Monster
{
public:
        Dragon(juce::Point<int> pos);
        virtual void attack() override;
        virtual void move() override;
protected:
        
        bool firestorm_used;
        Skill* firestorm;

};

class Projectile : public DynamicObject
{
public:
        Projectile(juce::Point<int> pos, juce::Point<int> dir);
        ~Projectile();

        virtual void move() override;
        virtual void on_death() override;
        virtual void collide(Character* target) override;
        virtual void collide(Projectile* target) override;
        virtual const juce::Image* get_sprite() const override;
        virtual Animation* get_explosion_animation() const = 0;

        int get_dmg() const;
        double get_dmg_mult() const;
        void set_damage(Stats user_stats);
protected:
        double dmg_mult;
        Animation* animation;
        juce::Point<int> dir;
};

class Fireball : public Projectile
{
public:
        Fireball(juce::Point<int> pos, juce::Point<int> dir);
        virtual Animation* get_explosion_animation() const;
};

class DarkFireball : public Projectile
{
public:
        DarkFireball(juce::Point<int> pos, juce::Point<int> dir);
        virtual Animation* get_explosion_animation() const;
};

} // End game_object