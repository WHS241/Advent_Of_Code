#ifndef DAY_17
#define DAY_17

#include <fstream>
#include <iostream>
#include <unordered_set>

#include <util/pair_hash.h>

int day_17_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_map<std::pair<std::size_t, std::size_t>, char,
                       util::pair_hash<std::size_t, std::size_t>>
      noted;
    constexpr char SAND('#'), SEEP('|'), SETTLE('~');

    for (std::string line; std::getline(reader, line);) {
        bool fix_x = line.front() == 'x';
        std::size_t fixed = std::stoul(line.substr(2, line.find(',') - 2));
        std::size_t pos = line.find(',');
        std::size_t ellipsis = line.find('.');

        std::size_t lower, upper;
        lower = std::stoul(line.substr(pos + 4, ellipsis - pos - 4));
        upper = std::stoul(line.substr(ellipsis + 2));

        for (std::size_t i = lower; i <= upper; ++i)
            if (fix_x)
                noted.emplace(std::make_pair(fixed, i), SAND);
            else
                noted.emplace(std::make_pair(i, fixed), SAND);
    }

    std::size_t minX = noted.begin()->first.first, maxX = noted.begin()->first.first,
                minY = noted.begin()->first.second, maxY = noted.begin()->first.second;
    for (auto p : noted) {
        minX = std::min(minX, p.first.first);
        maxX = std::max(maxX, p.first.first);
        minY = std::min(minY, p.first.second);
        maxY = std::max(maxY, p.first.second);
    }
    --minX;
    ++maxX;

    std::pair<std::size_t, std::size_t> start(500, 0);
    noted[start] = SEEP;

    std::vector<std::pair<std::size_t, std::size_t>> process;
    process.push_back(start);

    while (!process.empty()) {
        auto curr = process.back();
        process.pop_back();

        if (curr.second == maxY)
            continue;
        if (!noted.contains(curr) || noted[curr] != SEEP)
            continue;

        if (!noted.contains({curr.first, curr.second + 1})) {
            process.emplace_back(curr.first, curr.second + 1);
            noted[process.back()] = SEEP;
            continue;
        }

        if (noted[{curr.first, curr.second + 1}] == SEEP) // already processed
            continue;

        // on top of sand or settled water
        bool left_flow = false, right_flow = false;
        std::size_t left_bound = curr.first - 1, right_bound = curr.first + 1;
        while (!left_flow
               && (!noted.contains({left_bound, curr.second})
                   || noted[{left_bound, curr.second}] != SAND)) {
            if (!noted.contains({left_bound, curr.second + 1})
                || noted[{left_bound, curr.second + 1}] == SEEP)
                left_flow = true;
            else
                --left_bound;
        }

        while (!right_flow
               && (!noted.contains({right_bound, curr.second})
                   || noted[{right_bound, curr.second}] != SAND)) {
            if (!noted.contains({right_bound, curr.second + 1})
                || noted[{right_bound, curr.second + 1}] == SEEP)
                right_flow = true;
            else
                ++right_bound;
        }

        if (!left_flow && !right_flow) {
            for (std::size_t i = left_bound + 1; i < right_bound; ++i) {
                noted[{i, curr.second}] = SETTLE;
                process.emplace_back(i, curr.second - 1);
            }
            continue;
        }

        for (std::size_t i = left_bound + 1; i < right_bound; ++i)
            noted[{i, curr.second}] = SEEP;
        if (left_flow) {
            process.emplace_back(left_bound, curr.second);
            noted[process.back()] = SEEP;
        }
        if (right_flow) {
            process.emplace_back(right_bound, curr.second);
            noted[process.back()] = SEEP;
        }
    }

    std::size_t visited(0), settled(0);
    for (std::size_t i = minY; i <= maxY; ++i) {
        for (std::size_t j = minX; j <= maxX; ++j) {
            if (noted.contains({j, i}) && noted[{j, i}] != SAND) {
                ++visited;
                if (noted[{j, i}] == SETTLE)
                    ++settled;
            }
        }
    }
    std::cout << visited << std::endl << settled << std::endl;

    return 0;
}

#endif // DAY_17
