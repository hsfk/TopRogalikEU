#include "Animation.h"

Animation::Animation(const Frames* frames, unsigned fps, int cycles, bool started, int align)
        : frames(frames), fps(fps), cycles(cycles), align(align)
{
        if (started)
                start();
}

int Animation::get_align()
{
        return align;
}

bool Animation::is_done()
{
        return get_frame_count_from_start() > ((frames->size() - 1) * cycles);
}

void Animation::start()
{
        start_time = Time::currentTimeMillis();
}

const juce::Image* Animation::get_frame() const
{
        int64 t = get_frame_count_from_start();
        t %= frames->size();
        return (*frames)[get_frame_count_from_start() % frames->size()];
}

int64 Animation::get_frame_count_from_start() const
{
        return (((Time::currentTimeMillis() - start_time) * fps) / 1000);
}

Frames::Frames(const juce::Image& asset, unsigned frames)
        :Frames(asset, frames, 1)
{
}

Frames::Frames(const juce::Image& asset, unsigned width, unsigned height)
{
        unsigned frame_width = asset.getWidth() / width;
        unsigned frame_height = asset.getHeight() / height;
        for (unsigned i = 0; i < height; i++)
                for (unsigned j = 0; j < width; j++)
                {
                        juce::Rectangle<int> area(j * frame_width, i * frame_height , frame_width, frame_height);
                        this->push_back(new juce::Image(asset.getClippedImage(area).createCopy()));
                }
}

AnimationList::AnimationList(bool has_mutex)
{
        locker = nullptr;
        if (has_mutex)
                locker = new CriticalSection();
}

AnimationList::~AnimationList()
{
        size_t size = this->size();
        for (size_t i = 0; i < size; i++)
        {
                Animation* animation = this->pop();
                delete animation;
        }
        if (locker)
                delete locker;
}

size_t AnimationList::size() const
{
        return this->std::list<Animation*>::size();
}

void AnimationList::push(Animation* node)
{
        if (locker)
                locker->enter();

        this->push_back(node);

        if (locker)
                locker->exit();
}

Animation* AnimationList::pop()
{
        if (locker)
                locker->enter();

        Animation* result = this->front();
        this->pop_front();

        if (locker)
                locker->exit();

        return result;
}
