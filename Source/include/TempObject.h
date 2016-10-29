#pragma once

#include "GameObject.h"
#include "Items.h"

namespace game_object
{

class TempObject : public GameObject
{
public:
        TempObject(juce::Point<int> pos, unsigned lifetime = -1);

        virtual void collide(Character* target) = 0;
        virtual void move() override;
        virtual void on_death() override;
protected:
        unsigned lifetime;
        void delete_from_map();
};

class Portal : public TempObject
{
public:
        Portal(juce::Point<int> pos);
        virtual void collide(Character* target) override;
        virtual const juce::Image* get_sprite() const override;
        void bind(Portal* dest);
protected:
        Portal* dest;
        Animation* anim;
};

class Drop : public TempObject
{
public:
        Drop(juce::Point<int> pos, Item* item);

        virtual void collide(Character* target) override;
        virtual const juce::Image* get_sprite() const override;
        Item* get_item() const;
protected:
        Item* item;
};

class Firestorm : public TempObject
{
public:
        Firestorm(juce::Point<int> pos, int latency = 1);
        virtual void collide(Character* target) override {}
        virtual const juce::Image* get_sprite() const override;

        void set_dmg(Stats user);
        virtual void on_death() override;
protected:
        int dmg;
};

} // End game_object