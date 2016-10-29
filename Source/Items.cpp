#include "Items.h"
#include "Resource.h"
#include "DynamicObject.h"
#include "GlobalData.h"

Item::Item(const juce::Image* icon, bool stackable)
        :icon(icon), stackable(stackable), count(1), id(-1)
{
}

const juce::Image* Item::get_icon() const
{
        return icon;
}

bool Item::is_stackable() const
{
        return stackable;
}

int Item::get_id() const
{
        return id;
}

int Item::get_count() const
{
        return count;
}

void Item::set_count(int count)
{
        this->count = count;
}

HealPotion::HealPotion(const juce::Image* icon, int heal_amount, int id)
        : Item(icon, true), heal_amount(heal_amount)
{
        this->id = id;
}

void HealPotion::use(game_object::Character* user)
{
        user->get_gdata()->get_map()->add_animation(new OneTimeAnimation(Resource::get_heal_frames()), user->get_pos());
        user->heal(heal_amount);
        count--;
}

SmallHealPotion::SmallHealPotion()
        : HealPotion(Resource::get_small_heal_potion_sprite(), 40, small_heal_potion)
{
}

MediumHealPotion::MediumHealPotion()
        : HealPotion(Resource::get_medium_heal_potion_sprite(), 60, medium_heal_potion)
{
}

LargeHealPotion::LargeHealPotion()
        : HealPotion(Resource::get_large_heal_potion_sprite(), 90, large_heal_potion)
{
}

GreatHealPotion::GreatHealPotion()
        : HealPotion(Resource::get_great_heal_potion_sprite(), 135, great_heal_potion)
{
}

Weapon::Weapon(const juce::Image* icon, Skill* skill)
        :Item(icon), skill(skill)
{
}

void Weapon::use(game_object::Character* user)
{
        skill->use(user);
}

HurricaneSword::HurricaneSword()
        : Weapon(Resource::get_hurricane_sword_sprite(), new BladeVortex())
{
}

PyromancyFlame::PyromancyFlame()
        : Weapon(Resource::get_pyromancy_flame_sprite(), new PlayerFireball())
{
}

DarkPyromancyFlame::DarkPyromancyFlame()
        : Weapon(Resource::get_dark_pyromancy_flame_sprite(), new PlayerDarkFireball())
{
}

BlinkDagger::BlinkDagger()
        : Weapon(Resource::get_blink_dagger_sprite(), new PlayerFlickerStrike())
{
}

DarkfireScroll::DarkfireScroll()
        : Weapon(Resource::get_darkfire_scroll_sprite(), new Darkfire())
{
}

FirestormScroll::FirestormScroll()
        : Weapon(Resource::get_firestorm_scroll_sprite(), new PlayerFirestorm())
{
}
