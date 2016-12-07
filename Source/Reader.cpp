#include "Reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "resources/default_map.h"

GameMap* Reader::read_map_from_file(const std::string& file)
{
        std::ifstream in(file.c_str());
        return read_map_from_file(in);
}

GameMap* Reader::read_map_from_file(std::ifstream& in)
{
        if (!in.is_open())
                return nullptr;

        std::vector<std::string> data;
        while (!in.eof())
        {
                std::string line;
                std::getline(in, line);
                while(line.size() == 0 && !in.eof())
                        std::getline(in, line);
                data.push_back(line);
        }
        return new GameMap(data);
}

GameMap* Reader::read_map_from_str(const char* map)
{
        juce::StringArray lines;
        lines.addTokens(map, "\n", "");
    
        std::vector<std::string> data;
        for (int i = 0; i < lines.size(); i++)
        {
                if (lines[i].length() == 0)
                        continue;
                std::string l;
                l.reserve(lines[i].length());
                for (int j = 0; j < lines[i].length(); j++)
                        l.push_back(lines[i][j]);
                data.push_back(l);
        }
        return new GameMap(data);
}

GameMap* Reader::get_default_map()
{
        return read_map_from_str((char*)default_map);
}
