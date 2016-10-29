#include "Reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>


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