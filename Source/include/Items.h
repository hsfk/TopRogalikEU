#pragma once

#include <JuceHeader.h>
#include "GameObject.h"
#include "Skill.h"

enum ItemIDs
{
        small_heal_potion,
        medium_heal_potion,
        large_heal_potion,
        great_heal_potion
};

class Item
{
public:
        Item(const juce::Image* icon, bool stackable = false);

        virtual void use(game_object::Character* user) = 0;
        const juce::Image* get_icon() const;
        bool is_stackable() const;
        int get_id() const;
        int get_count() const;
        void set_count(int count);
protected:
        int id;
        int count;
        bool stackable;
        const juce::Image* icon;
};

class HealPotion : public Item
{
public:
        HealPotion(const juce::Image* icon, int heal_amount, int id = -1);
        virtual void use(game_object::Character* user) override;
protected:
        int heal_amount;
};

class SmallHealPotion : public HealPotion
{
public:
        SmallHealPotion();
};

class MediumHealPotion : public HealPotion
{
public:
        MediumHealPotion();
};

class LargeHealPotion : public HealPotion
{
public:
        LargeHealPotion();
};

class GreatHealPotion : public HealPotion
{
public:
        GreatHealPotion();
};

class Weapon : public Item
{
public:
        Weapon(const juce::Image* icon, Skill* skill);
        virtual void use(game_object::Character* user) override;
protected:
        Skill* skill;
};

class BlinkDagger : public Weapon
{
public:
        BlinkDagger();
};

class HurricaneSword : public Weapon
{
public:
        HurricaneSword();
};

class PyromancyFlame : public Weapon
{
public:
        PyromancyFlame();
};

class DarkPyromancyFlame : public Weapon
{
public:
        DarkPyromancyFlame();
};

class DarkfireScroll : public Weapon
{
public:
        DarkfireScroll();
};

class FirestormScroll : public Weapon
{
public:
        FirestormScroll();
};