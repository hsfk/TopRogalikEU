#pragma once

#include "Animation.h"

class CharacterSprite
{
public:
        CharacterSprite(const Frames* frames);

        const juce::Image* get_left() const;
        const juce::Image* get_right() const;
        const juce::Image* get_front() const;
        const juce::Image* get_rear() const;
protected:
        const Frames* frames;

        enum SpriteFrame
        {
                front,
                left,
                right,
                rear
        };
};