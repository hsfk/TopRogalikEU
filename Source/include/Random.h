#pragma once

#include <random>

class RNG
{
public:
        static int next_int();
        static long long next_int64();
        static unsigned next_uint();
        static bool next_bool();
protected:
        static std::random_device rd;
};