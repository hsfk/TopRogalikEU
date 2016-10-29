#pragma once

#include "GameObject.h"
#include "Animation.h"

class Skill
{
public:
        Skill(const Frames* frames);
        virtual void use(game_object::Character* user);
protected:
        void add_animation(game_object::Character* user);
        void deal_damage(game_object::Character* user, juce::Point<int> pos, int dmg);
        bool enough_stats(game_object::Character* user);
        void burn_stats(game_object::Character* user);

        int mana_cost;
        int hp_cost;
        const Frames* frames;
};

class BladeVortex : public Skill
{
public:
        BladeVortex();
        virtual void use(game_object::Character* user) override;
};

class FireballSkill : public Skill
{
public:
        FireballSkill();
        virtual void use(game_object::Character* user) override = 0;
protected:
        void create_projectile(game_object::Character* user, game_object::Projectile* p);
        void create_fireball(game_object::Character* user, juce::Point<int> dir);
};

class PlayerFireball : public FireballSkill
{
public:
        PlayerFireball();
        virtual void use(game_object::Character* user) override;
};

class MonsterFireball : public FireballSkill
{
public:
        MonsterFireball();
        virtual void use(game_object::Character* user) override;
};

class DarkFireball : public FireballSkill
{
public:
        DarkFireball();
        virtual void use(game_object::Character* user) override;
};

class PlayerDarkFireball : public DarkFireball
{
public:
        PlayerDarkFireball();
        virtual void use(game_object::Character* user) override;
};

class FlickerStrike : public Skill
{
public:
        FlickerStrike();
protected:
        virtual void use(game_object::Character* user, game_object::DynamicObject* target);
};

class PlayerFlickerStrike : public FlickerStrike
{
public:
        PlayerFlickerStrike();
        virtual void use(game_object::Character* user) override;
protected:
        game_object::DynamicObject* get_random_enemy(game_object::Character* user);
        juce::Random random;
};

class MonsterFlickerStrike : public FlickerStrike
{
public:
        MonsterFlickerStrike();
        virtual void use(game_object::Character* user) override;
};

class Darkfire : public Skill
{
public:
        Darkfire();
        virtual void use(game_object::Character* user) override;
};

class FirestormSkill : public Skill
{
public:
        FirestormSkill();
        virtual void use(game_object::Character* user) override;
protected:
        void create_firestorm(game_object::Character* user, juce::Point<int> pos, int latency = 1);
        std::vector<juce::Point<int>> get_random_positions_in_vision(game_object::Character* user, int count) const;
};

class PlayerFirestorm : public FirestormSkill
{
public:
        PlayerFirestorm();
        virtual void use(game_object::Character* user) override;
};

class DragonFirestorm : public FirestormSkill
{
public:
        DragonFirestorm();
        virtual void use(game_object::Character* user) override;
};