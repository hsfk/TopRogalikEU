#include "Random.h"

std::random_device RNG::rd;

long long RNG::next_int64()
{
        std::mt19937_64 gen(rd());
        return gen();
}

int RNG::next_int()
{
        return (int)next_int64();
}

unsigned RNG::next_uint()
{
        return (unsigned)next_int64();
}

bool RNG::next_bool()
{
        return next_uint() % 2 == 0;
}
