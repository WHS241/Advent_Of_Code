#ifndef DAY_22
#define DAY_22 2

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>

#include <util/pair_hash.h>

int day_22_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    constexpr char INFECTED = '#';
    std::ifstream reader(argv[1]);
    long i(0), j(0);
    std::unordered_set<std::pair<long, long>, util::pair_hash<long, long>> infected;

    for (std::string line; std::getline(reader, line); ++i)
        for (j = 0; j < line.size(); ++j)
            if (line[j] == INFECTED)
                infected.emplace(i, j);

    long x(i / 2), y(j / 2);

    std::size_t steps        = std::stoul(argv[2]);
    std::size_t direction    = 0;
    std::size_t num_infected = 0;
#if DAY_22 == 2
    std::unordered_set<std::pair<long, long>, util::pair_hash<long, long>> weakened, flagged;
#endif

    for (std::size_t i = 0; i < steps; ++i) {
#if DAY_22 == 1
        if (infected.count({x, y})) {
            infected.erase({x, y});
            direction += 1;
        } else {
            infected.emplace(x, y);
            ++num_infected;
            direction += 3;
        }
#elif DAY_22 == 2
        if (flagged.count({x, y})) {
            flagged.erase({x, y});
            direction += 2;
        } else if (infected.count({x, y})) {
            flagged.emplace(x, y);
            infected.erase({x, y});
            direction += 1;
        } else if (weakened.count({x, y})) {
            infected.emplace(x, y);
            weakened.erase({x, y});
            ++num_infected;
        } else {
            weakened.emplace(x, y);
            direction += 3;
        }
#endif

        direction %= 4;
        switch (direction) {
        case 0:
            --x;
            break;

        case 1:
            ++y;
            break;

        case 2:
            ++x;
            break;

        case 3:
            --y;
            break;

        default:
            break;
        }
    }

    std::cout << num_infected << std::endl;

    return 0;
}

#endif // DAY_22
