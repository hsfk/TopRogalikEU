#include "App.h"
#include "Components.h"
#include "Reader.h"
#include "Resource.h"
#include "GlobalData.h"
#include "Settings.h"

MainWindow::MainWindow(String name) : DocumentWindow(name, Colours::lightgrey, DocumentWindow::allButtons)
{
        setUsingNativeTitleBar(true);
        setResizable(false, false);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
}

void MainWindow::closeButtonPressed()
{
        JUCEApplication::getInstance()->systemRequestedQuit();
}

TopRogalikEUApplication::TopRogalikEUApplication()
{
}

const juce::String TopRogalikEUApplication::getApplicationName()
{
        return ProjectInfo::projectName;
}

const juce::String TopRogalikEUApplication::getApplicationVersion()
{
        return ProjectInfo::versionString;
}

bool TopRogalikEUApplication::moreThanOneInstanceAllowed()
{
        return true;
}

void TopRogalikEUApplication::initialise(const juce::String& commandLine)
{
        Resource::initialize();
        Settings::initialize("settings.cfg");
        GameMap* map = Reader::read_map_from_file(Settings::Game::get_map());
        if (!map)
                quit();
        GlobalState* gstate = new GlobalState(map);
        MainContentComponent* comp = new MainContentComponent(gstate);
        game_manager = new GameManager(comp, gstate);
        mainWindow = new MainWindow(getApplicationName());
        mainWindow->setContentOwned(comp, true);
        mainWindow->centreWithSize(comp->getWidth(), comp->getHeight());
        comp->grabKeyboardFocus();
        game_manager->game_start();
}

void TopRogalikEUApplication::shutdown()
{
        mainWindow = nullptr;
        delete game_manager;
}

void TopRogalikEUApplication::systemRequestedQuit()
{
        quit();
}

void TopRogalikEUApplication::anotherInstanceStarted(const juce::String& commandLine)
{
}
