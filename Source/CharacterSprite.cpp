#include "CharacterSprite.h"

CharacterSprite::CharacterSprite(const Frames* frames) : frames(frames)
{
}

const Image* CharacterSprite::get_left() const
{
        return (*frames)[left];
}

const Image* CharacterSprite::get_right() const
{
        return (*frames)[right];
}

const Image* CharacterSprite::get_front() const
{
        return (*frames)[front];
}

const Image* CharacterSprite::get_rear() const
{
        return (*frames)[rear];
}