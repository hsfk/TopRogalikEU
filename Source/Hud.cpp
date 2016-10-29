#include "Hud.h"

Hud::Hud(const juce::Image* left, const juce::Image* right,
        const juce::Image* health_orb, const juce::Image* mana_orb)
        : left(left), right(right), health_orb(health_orb), mana_orb(mana_orb)
{
        healh_orb_buffer = new juce::Image(health_orb->createCopy());
        mana_orb_buffer = new juce::Image(mana_orb->createCopy());
}

const juce::Image* Hud::get_left() const
{
        return left;
}

const juce::Image* Hud::get_right() const
{
        return right;
}

const juce::Image* Hud::get_health_orb(float perc) const
{
        make_upper_part_transparent(healh_orb_buffer, health_orb, perc);
        return healh_orb_buffer;
}

const juce::Image* Hud::get_mana_orb(float perc) const
{
        make_upper_part_transparent(mana_orb_buffer, mana_orb, perc);
        return mana_orb_buffer;
}

void Hud::make_upper_part_transparent(juce::Image* buffer, const juce::Image* orig, float perc) const
{
        Graphics g(*buffer);
        g.drawImageAt(*orig, 0, 0);
        if (perc > 100.0)
                perc = 100.0;
        int rem = 100 - (int)(perc * 100.0);
        buffer->clear(juce::Rectangle<int>(0, 0, buffer->getWidth(), (buffer->getHeight() * rem) / 100),
                juce::Colours::transparentBlack);
}
