#pragma once

#include "Map.h"

class Reader
{
public:
        static GameMap* read_map_from_file(const std::string& file);
        static GameMap* read_map_from_file(std::ifstream& in);
};