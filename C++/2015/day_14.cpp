#ifndef DAY_14
#define DAY_14

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

int day_14_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    struct info {
        std::size_t max_speed;
        std::size_t runtime;
        std::size_t rest;
    };

    std::ifstream reader(argv[1]);
    std::size_t duration = std::stoul(argv[2]);

    std::unordered_map<std::string, info> reindeers;
    std::unordered_map<std::string, std::size_t> dist, points;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream reader(line);
        std::string name;
        info stats;
        reader >> name >> stats.max_speed >> stats.runtime >> stats.rest;
        reindeers.emplace(name, stats);
        dist.emplace(name, 0UL);
        points.emplace(name, 0UL);
    }

    for (std::size_t i = 0; i < duration; ++i) {
        for (const std::pair<const std::string, info>& d : reindeers) {
            if (i % (d.second.runtime + d.second.rest) < d.second.runtime) {
                dist[d.first] += d.second.max_speed;
            }
        }

        std::size_t lead = 0;
        for (const std::pair<const std::string, std::size_t>& d : dist)
            lead = std::max(lead, d.second);
        for (std::pair<const std::string, std::size_t>& d : points)
            if (dist[d.first] == lead)
                ++d.second;
    }

    std::cout << "Part 1: \n";
    for (const std::pair<const std::string, std::size_t>& d : dist)
        std::cout << d.first << ": " << d.second << '\n';

    std::cout << "\nPart 2: \n";
    for (const std::pair<const std::string, std::size_t>& d : points)
        std::cout << d.first << ": " << d.second << '\n';

    return 0;
}

#endif // DAY_14
