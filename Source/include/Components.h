#pragma once

#include <JuceHeader.h>
#include "GlobalData.h"

class MainContentComponent : public AnimatedAppComponent
{
public:
        MainContentComponent(GlobalState* gstate);
        ~MainContentComponent();

        void update() override;
        void paint(Graphics& g) override;
        void resized() override;
private:
        GlobalState* gstate;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};