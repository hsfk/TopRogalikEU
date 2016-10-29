#pragma once

#include "Common.h"
#include "Components.h"
#include "KeyEvents.h"
#include <vector>

class GameManager : protected juce::KeyListener, protected juce::Timer
{
public:
        GameManager(MainContentComponent* main_window, GlobalState* gdata);
        ~GameManager() override;

        void game_start();
        void game_stop();
protected:
        friend KeyEvents;
        enum States
        {
                game,
                inventory,
                game_over
        };

        void next_cycle();

        bool keyPressed(const juce::KeyPress& key, juce::Component* origin) override;
        virtual void timerCallback() override;

        bool stopped;
        std::vector<GameState*> states;
        MainContentComponent* main_window;
        GlobalState* gdata;
};