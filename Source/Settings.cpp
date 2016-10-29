#include "Settings.h"
#include <iostream>
#include <fstream>

int Settings::pseudo_real_time = 0;
int Settings::screen_width = 1000;
int Settings::screen_height = 800;
uint8_t Settings::wall_transparency = 80;
std::string Settings::map = "map.txt";


void* get_ptr(std::unordered_map<uint64_t, void*>& values, uint64_t key)
{
        std::unordered_map<uint64_t, void*>::iterator it = values.find(key);
        return it == values.end() ? nullptr : it->second;
}

void Settings::initialize(std::string file)
{
        std::ifstream in(file.c_str());
        if (!in.is_open())
                return;
        Parser p(in);
        std::unordered_map<uint64_t, void*> values = p.get_values();

#define get_value(VAR, TY) VAR = get_ptr(values, juce::String(#VAR).hashCode64()) == nullptr ?\
        VAR : *(TY*)get_ptr(values, juce::String(#VAR).hashCode64())

        get_value(pseudo_real_time, int);
        get_value(screen_width, int);
        get_value(screen_height, int);
        get_value(wall_transparency, int);
        get_value(map, std::string);
}

uint8_t Settings::Graphics::get_wall_transparency()
{
        return wall_transparency;
}

int Settings::Graphics::get_screen_width()
{
        return screen_width;
}

int Settings::Graphics::get_screen_height()
{
        return screen_height;
}

std::string Settings::Game::get_map()
{
        return map;
}

bool Settings::Game::is_pseudo_real_time()
{
        return pseudo_real_time != 0;
}


Parser::Parser(std::ifstream& in)
{
        while (!in.eof())
        {
                std::string line;
                std::getline(in, line);
                tokens.addTokens(juce::String(line.c_str()), " ", "\"");
        }
}

std::unordered_map<uint64_t, void*> Parser::get_values()
{
        std::unordered_map<uint64_t, void*> result;
        for (int i = 0; i < tokens.size(); i++)
        {
                std::pair<uint64_t, void*> p;
                p.first = tokens[i++].hashCode64();
                if (i == tokens.size())
                        break;

                if (tokens[i].isQuotedString())
                        p.second = new std::string(tokens[i].substring(1, tokens[i].length() - 1).getCharPointer());
                else if (tokens[i].containsOnly("0123456789"))
                        p.second = new int(tokens[i].getLargeIntValue());
                else
                        p.second = nullptr;
                result.insert(p);
        }
        return result;
}
