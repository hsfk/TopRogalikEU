#include "Components.h"
#include "Engine.h"
#include "GameState.h"
#include "Settings.h"

MainContentComponent::MainContentComponent(GlobalState* gstate)
        : gstate(gstate)
{
        int width = Settings::Graphics::get_screen_width();
        int height = Settings::Graphics::get_screen_height();
        int cell_size = 25;

        setSize(width, height);
        Engine::initialize(gstate->get_map(), width / cell_size, height / cell_size, cell_size);
        setFramesPerSecond(60);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::update()
{
}

void MainContentComponent::paint(Graphics& g)
{
        g.drawImageAt(*gstate->get_current_state()->render(), 0, 0);
}

void MainContentComponent::resized()
{
}

