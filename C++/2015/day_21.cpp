#ifndef DAY_21
#define DAY_21 2

#include <climits>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <list>
#include <numeric>
#include <sstream>
#include <string>

constexpr std::size_t HERO_HP = 100;
constexpr std::size_t BOSS_HP = 100;
constexpr std::size_t BOSS_DMG = 8;
constexpr std::size_t BOSS_DEF = 2;

struct equipment {
    std::string name;
    std::size_t cost;
    std::size_t damage;
    std::size_t armor;
};

bool success(const std::initializer_list<equipment>& bought) {
    std::size_t hero_dmg(0), hero_def(0);
    for (const equipment& e : bought) {
        hero_dmg += e.damage;
        hero_def += e.armor;
    }

    hero_dmg = std::max(hero_dmg, BOSS_DEF + 1);
    hero_def = std::min(hero_def, BOSS_DMG - 1);

    std::size_t hero_to_boss = hero_dmg - BOSS_DEF;
    std::size_t boss_to_hero = BOSS_DMG - hero_def;

    return (BOSS_HP / hero_to_boss + (BOSS_HP % hero_to_boss != 0))
        <= (HERO_HP / boss_to_hero + (HERO_HP % boss_to_hero != 0));
}

int day_21_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::string current;

    std::list<equipment> weapons, armor, rings;
    armor.push_back({"", 0, 0, 0});
    rings.push_back({"", 0, 0, 0});
    rings.push_back({"", 0, 0, 0});

    for (std::string line; std::getline(reader, line);) {
        if (current.empty() || line.empty())
            current = line.substr(0, line.find(':'));
        else {
            std::istringstream reader(line);
            equipment e;
            reader >> e.name >> e.cost >> e.damage >> e.armor;
            if (current == "Weapons")
                weapons.push_back(e);
            else if (current == "Armor")
                armor.push_back(e);
            else if (current == "Rings")
                rings.push_back(e);
        }
    }

    std::size_t min_cost = ULONG_MAX;
    std::size_t max_cost = 0;
    for (const equipment& w : weapons) {
        for (const equipment& a : armor) {
            for (auto it = rings.begin(); it != rings.end(); ++it) {
                for (auto it2 = it; it2 != rings.end(); ++it2) {
                    if (it2 != it) {
                        std::size_t cost = w.cost + a.cost + it->cost + it2->cost;
                        if (cost < min_cost && success({w, a, *it, *it2}))
                            min_cost = cost;
                        if (cost > max_cost && !success({w, a, *it, *it2}))
                            max_cost = cost;
                    }
                }
            }
        }
    }

    std::cout << min_cost << '\n' << max_cost << std::endl;

    return 0;
}

#endif // DAY_21
