#pragma once

#include "Map.h"

class Reader
{
public:
        static GameMap* read_map_from_file(const std::string& file);
        static GameMap* read_map_from_file(std::ifstream& in);
        static GameMap* read_map_from_str(const char* map);
        static GameMap* get_default_map();
};
