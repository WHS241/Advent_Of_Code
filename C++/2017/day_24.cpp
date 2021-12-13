#ifndef DAY_24
#define DAY_24 2

#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

int day_24_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<std::pair<std::size_t, std::size_t>> parts;

    for (std::string line; std::getline(reader, line);) {
        std::size_t delim = line.find('/');
        parts.emplace_back(std::stoul(line.substr(0, delim)), std::stoul(line.substr(delim + 1)));
    }

    std::size_t max_strength(0), max_length(0), max_l_str(0);
    std::vector<std::tuple<std::vector<bool>, std::size_t, std::size_t, std::size_t>> frontier;
    frontier.emplace_back(std::vector<bool>(parts.size(), false), 0, 0, 0);

    while (!frontier.empty()) {
        std::vector<bool> used;
        std::size_t strength, last, length;
        std::tie(used, strength, last, length) = frontier.back();
        frontier.pop_back();
        max_strength = std::max(max_strength, strength);
        if (max_length == length) {
            max_l_str = std::max(max_l_str, strength);
        } else if (max_length < length) {
            max_length = length;
            max_l_str = strength;
        }

        for (std::size_t i = 0; i < parts.size(); ++i) {
            if (!used[i]) {
                if (parts[i].first == last) {
                    std::vector<bool> next(used);
                    next[i] = true;
                    frontier.emplace_back(std::move(next),
                                          strength + parts[i].first + parts[i].second,
                                          parts[i].second, length + 1);
                } else if (parts[i].second == last) {
                    std::vector<bool> next(used);
                    next[i] = true;
                    frontier.emplace_back(std::move(next),
                                          strength + parts[i].first + parts[i].second,
                                          parts[i].first, length + 1);
                }
            }
        }
    }

#if DAY_24 == 1
    std::cout << max_strength << std::endl;
#elif DAY_24 == 2
    std::cout << max_l_str << std::endl;
#endif

    return 0;
}

#endif // DAY_24
