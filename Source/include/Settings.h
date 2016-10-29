#pragma once

#include <JuceHeader.h>
#include <string>
#include <unordered_map>

class Parser
{
public:
        Parser(std::ifstream& in);

        std::unordered_map<uint64_t, void*> get_values();
protected:
        StringArray tokens;
};

class Settings
{
public:
        static void initialize(std::string file);

        class Graphics
        {
        public:
                static uint8_t get_wall_transparency();
                static int get_screen_width();
                static int get_screen_height();
        };

        class Game
        {
        public:
                static std::string get_map();
                static bool is_pseudo_real_time();
        };

protected:
        static int pseudo_real_time;
        static int screen_width;
        static int screen_height;
        static uint8_t wall_transparency;
        static std::string map;
};