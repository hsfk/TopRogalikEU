#pragma once

#include <JuceHeader.h>
#include "GameManager.h"

class MainWindow : public juce::DocumentWindow
{
public:
        MainWindow(juce::String name);

        void closeButtonPressed() override;
private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

class TopRogalikEUApplication : public juce::JUCEApplication
{
public:
        TopRogalikEUApplication();

        const juce::String getApplicationName() override;
        const juce::String getApplicationVersion() override;
        bool moreThanOneInstanceAllowed() override;
        void initialise(const juce::String& commandLine) override;
        void shutdown() override;
        void systemRequestedQuit() override;
        void anotherInstanceStarted(const juce::String& commandLine) override;
private:
        juce::ScopedPointer<MainWindow> mainWindow;
        GameManager* game_manager;
};