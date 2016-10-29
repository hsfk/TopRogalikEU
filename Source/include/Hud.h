#pragma once

#include <JuceHeader.h>

class Hud
{
public:
        Hud(const juce::Image* left, const juce::Image* right,
                const juce::Image* health_orb, const juce::Image* mana_orb);

        const juce::Image* get_left() const;
        const juce::Image* get_right() const;
        const juce::Image* get_health_orb(float perc) const;
        const juce::Image* get_mana_orb(float perc) const;
protected:
        void make_upper_part_transparent(juce::Image* buffer, const juce::Image* orig, float perc) const;

        const juce::Image* left;
        const juce::Image* right;
        const juce::Image* health_orb;
        const juce::Image* mana_orb;

        mutable juce::Image* healh_orb_buffer;
        mutable juce::Image* mana_orb_buffer;
};