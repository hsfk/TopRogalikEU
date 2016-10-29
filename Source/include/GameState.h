#pragma once

#include <JuceHeader.h>
#include "KeyEvents.h"
#include "KeyboardLayout.h"

class GameState
{
public:
        GameState(GameManager* gm);

        virtual const juce::Image* render() = 0;
        void key_press(const int key_code);
protected:
        KeyboardLayout<GameManager>* layout;
        GameManager* gm;
};

class Game : public GameState
{
public:
        Game(GameManager* gm);
        virtual const juce::Image* render() override;
};

class Inventory : public GameState
{
public:
        Inventory(GameManager* gm);
        virtual const juce::Image* render() override;
};

class GameOver : public GameState
{
public:
        GameOver(GameManager* gm);
        virtual const juce::Image* render() override;
};

class Menu : public GameState
{
public:
};
