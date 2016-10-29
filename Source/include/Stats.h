#pragma once

#include <JuceHeader.h>
#include "Random.h"

struct Stats
{
        int cur_hp;
        int max_hp;
        int cur_mana;
        int max_mana;
        int defence;
        int damage;
        int crit_chance;
        int vision;
        int mana_regen;

        Stats(int max_hp, int max_mana, int defence, int damage, int vision)
                : cur_hp(max_hp), max_hp(max_hp), cur_mana(max_mana), max_mana(max_mana),
                defence(defence), damage(damage), vision(vision), mana_regen(3),
                crit_chance(100) { } // 100 out of 1000
        Stats() : Stats(0, 0, 0, 0, 0) { }

        void regenerate()
        {
                cur_mana += mana_regen;
                cur_mana = cur_mana > max_mana ? max_mana : cur_mana;
        }

        int get_dmg() const 
        {
                return RNG::next_uint() % 1000 <= crit_chance ? damage * 2 : damage;
        }
};

const Stats static_object_stats(  100,   1,    1,   1,    1  );
const Stats dynamic_object_stats( 20,    20,   1,   5,    3  );
const Stats zombie_stats(         50,    0,    1,   7,    8  );
const Stats skeleton_stats(       70,    0,    3,   8,    10 );
const Stats demon_stats(          140,   0,    2,   13,   10 );
const Stats dragon_stats(         350,   0,    5,   20,   10 );
const Stats player_stats(         150,   100,  5,   10,   10 );

//const Stats player_stats(15000, 10000, 500, 1000, 1000);