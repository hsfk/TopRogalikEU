#include "AI.h"
#include "DynamicObject.h"
#include "GlobalData.h"
#include "Stats.h"
#include "Random.h"

CharacterAI::CharacterAI(game_object::Character* character)
        : character(character)
{
}

juce::Point<int> CharacterAI::get_next_pos() const
{
        return character->get_pos();
}

std::vector<juce::Point<int>> CharacterAI::get_route_to_player() const
{
        return character->get_gdata()->get_map()->get_route(character->get_pos(), get_player()->get_pos());
}

int CharacterAI::distance_to_player() const
{
        get_player()->get_pos();
        return abs(character->get_pos().getDistanceFrom(get_player()->get_pos()));
}

const game_object::Knight* CharacterAI::get_player() const
{
        return character->get_gdata()->get_map()->get_player();
}

juce::Point<int> NoneAI::next_move()
{
        return character->get_pos();
}

juce::Point<int> MonsterAI::next_move()
{
        return character->get_pos();
}

bool MonsterAI::player_in_vision() const
{
        return character->see(get_player()->get_pos());
}

juce::Point<int> MonsterAI::follow_player() const
{
        if (!player_in_vision() || RNG::next_uint() % 4 == 0)
                return character->get_pos();
        std::vector<juce::Point<int>> route = get_route_to_player();
        if (route.size() > 0)
                return route[0];
        return character->get_pos();
}

juce::Point<int> ZombieAI::next_move()
{
        character->attack();
        return follow_player();
}

juce::Point<int> DemonAI::next_move()
{
        character->attack();
        return get_next_pos();
}

juce::Point<int> DemonAI::get_next_pos() const
{
        if (!player_in_vision())
                return character->get_pos();
        if (distance_to_player() > character->get_stats().vision / 2)
        {
                // use fireball
                return character->get_pos();
        }
        return follow_player();
}

juce::Point<int> SkeletonAI::next_move()
{
        character->attack();
        return follow_player();
}

juce::Point<int> DragonAI::next_move()
{
        character->attack();
        return follow_player();
}
