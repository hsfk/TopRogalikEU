#pragma once

#include <JuceHeader.h>
#include <list>

struct Frames : public std::vector<juce::Image*>
{
        Frames(const juce::Image& asset, unsigned frames);
        Frames(const juce::Image& asset, unsigned width, unsigned height);
};

class Animation
{
public:
        enum Align
        {
                center,
                bottom
        };
        Animation(const Frames* frames, unsigned fps = 25, int cycles = -1, bool started = true, int align = center);

        int get_align();
        bool is_done();
        void start();
        virtual const juce::Image* get_frame() const;
protected:
        int64 get_frame_count_from_start() const;

        int align;
        const Frames* frames;
        int cycles;
        unsigned fps;
        int64 start_time;
};

class OneTimeAnimation : public Animation
{
public:
        OneTimeAnimation(const Frames* frames, unsigned fps = 25, bool started = true, int align = center)
                : Animation(frames, fps, 1, started, align) { }
};

class AnimationList : protected std::list<Animation*>
{
public:
        AnimationList(bool has_mutex = true);
        ~AnimationList();

        size_t size() const;
        void push(Animation* anim);
        Animation* pop();
protected:
        CriticalSection* locker;
};