#pragma once

#include <JuceHeader.h>
#include "Common.h"
#include "Stats.h"
#include <vector>
#include <memory>

namespace game_object
{

class StaticObject;
class Wall;
class CaveEntrance;
class Trap;
class Ground;

class DynamicObject;
class Character;
class Monster;
class Zombie;
class Demon;
class Knight;
class Princess;
class Projectile;
class Fireball;

class TempObject;
class Drop;

class GameObject
{
public:
        GameObject(Stats stats, juce::Point<int> pos, juce::Colour color)
                : stats(stats), pos(pos), color(color), gdata(nullptr) { }
        virtual ~GameObject() { }

        virtual void move();
        virtual void on_death();
        virtual void take_damage(int dmg);
        virtual void heal(int amount);
        virtual void collide(GameObject* target);
        virtual void collide(StaticObject* target);
        virtual void collide(DynamicObject* target);
        virtual void collide(TempObject* target);
        virtual const juce::Image* get_sprite() const;

        juce::Point<int> get_pos() const;
        juce::Colour get_color() const;
        void set_gdata(GlobalState* gdata);
        void set_pos(juce::Point<int> pos);
        void set_stats(Stats stats);
        GlobalState* get_gdata() const;
        const Stats& get_stats() const;
        double distance(juce::Point<int> pos) const;
protected:
        Stats stats;
        juce::Colour color;
        juce::Point<int> pos;
        GlobalState* gdata;
};

} // End game_object
