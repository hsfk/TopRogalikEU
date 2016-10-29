#pragma once

#include "GameObject.h"
#include "Animation.h"

namespace game_object
{

class StaticObject : public GameObject
{
public:
        StaticObject(juce::Point<int> pos, juce::Colour color = juce::Colours::darkgreen)
                : GameObject(static_object_stats, pos, color), sprite(nullptr), height(0) { }

        int get_height() const;
        bool hides_player() const;
        virtual void move() { }
        virtual void on_death() override;
        virtual const juce::Image* get_sprite() const;
        virtual void collide(GameObject* target) override { target->collide(this); }
        virtual void collide(DynamicObject* target) { }
        virtual void collide(Projectile* target);
        virtual bool breakable() const { return false; }
        virtual bool passable() const { return false; }
protected:
        const juce::Image* sprite;
        int height;
}; 

class Wall : public StaticObject
{
public:
        Wall(juce::Point<int> pos);
};

class BreakableWall : public Wall
{
public:
        BreakableWall(juce::Point<int> pos) : Wall(pos) { }
        
        virtual bool breakable() const { return true; }
};

class CaveEntrance : public StaticObject
{
public:
        CaveEntrance(juce::Point<int> pos) : StaticObject(pos, juce::Colours::grey) { }

        virtual void collide(DynamicObject* target) override { }
};

class Trap : public StaticObject
{
public:

};

class Ground : public StaticObject
{
public:
        Ground(juce::Point<int> pos);

        virtual void collide(DynamicObject* target) override;
        virtual void collide(Projectile* target) override;
        virtual bool passable() const override { return true; }
};

} // End game_object