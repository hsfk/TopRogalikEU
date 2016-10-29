#pragma once

#include "GameObject.h"

class CharacterAI
{
public:
        CharacterAI(game_object::Character* character);

        virtual juce::Point<int> next_move() = 0;
protected:
        virtual juce::Point<int> get_next_pos() const;
        std::vector<juce::Point<int>> get_route_to_player() const;
        int distance_to_player() const;

        const game_object::Knight* get_player() const;
        game_object::Character* character;
};

class NoneAI : public CharacterAI
{
public:
        NoneAI(game_object::Character* character) : CharacterAI(character) { }
        juce::Point<int> next_move() override;
};

class MonsterAI : public CharacterAI
{
public:
        MonsterAI(game_object::Character* character) : CharacterAI(character) { }
        juce::Point<int> next_move() override;
protected:
        bool player_in_vision() const;
        juce::Point<int> follow_player() const;
};

class ZombieAI : public MonsterAI
{
public:
        ZombieAI(game_object::Character* character) : MonsterAI(character) { }
        juce::Point<int> next_move() override;
};

class DemonAI : public MonsterAI
{
public:
        DemonAI(game_object::Character* character) : MonsterAI(character) { }
        juce::Point<int> next_move() override;
protected:
        virtual juce::Point<int> get_next_pos() const override;
};

class SkeletonAI : public MonsterAI
{
public:
        SkeletonAI(game_object::Character* character) : MonsterAI(character) { }
        juce::Point<int> next_move() override;
};

class DragonAI : public MonsterAI
{
public:
        DragonAI(game_object::Character* character) : MonsterAI(character) { }
        juce::Point<int> next_move() override;
};