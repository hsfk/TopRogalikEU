#include "GameObject.h"

void game_object::GameObject::move()
{
        stats.regenerate();
}

void game_object::GameObject::on_death()
{
        delete this;
}

void game_object::GameObject::collide(GameObject* target)
{
}

void game_object::GameObject::collide(StaticObject* target)
{
}

void game_object::GameObject::collide(DynamicObject* target)
{
}

void game_object::GameObject::collide(TempObject* target)
{
}

juce::Point<int> game_object::GameObject::get_pos() const
{
        return this->pos;
}

juce::Colour game_object::GameObject::get_color() const
{
        return this->color;
}

void game_object::GameObject::set_gdata( GlobalState* gdata)
{
        this->gdata = gdata;
}

void game_object::GameObject::set_pos(juce::Point<int> pos)
{
        this->pos = pos;
}

void game_object::GameObject::set_stats(Stats stats)
{
        this->stats = stats;
}

void game_object::GameObject::take_damage(int dmg)
{
        dmg -= this->stats.defence;
        if (dmg <= 0)
                dmg = 1;
        this->stats.cur_hp -= dmg;
        if (this->stats.cur_hp <= 0)
                on_death();
}

void game_object::GameObject::heal(int amount)
{
        amount = abs(amount);
        this->stats.cur_hp += amount;
        if (this->stats.cur_hp > this->stats.max_hp)
                this->stats.cur_hp = this->stats.max_hp;
}

GlobalState* game_object::GameObject::get_gdata() const
{
        return this->gdata;
}

const Stats& game_object::GameObject::get_stats() const
{
        return this->stats;
}

double game_object::GameObject::distance(juce::Point<int> pos) const
{
        double dx = abs(pos.x - this->pos.x);
        double dy = abs(pos.y - this->pos.y);
        return sqrt(dx * dx + dy * dy);
}

const juce::Image* game_object::GameObject::get_sprite() const
{
        return nullptr;
}
