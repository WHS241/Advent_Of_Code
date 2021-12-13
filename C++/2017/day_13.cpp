#ifndef DAY_13
#define DAY_13 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t severity = 0;

    std::unordered_map<std::size_t, std::size_t> periods;

    for (std::string line; std::getline(reader, line);) {
        std::size_t delim  = line.find(':');
        std::size_t depth  = std::stoul(line.substr(0, delim));
        std::size_t range  = std::stoul(line.substr(delim + 1));
        std::size_t period = 2 * (range - 1);

        periods[depth] = period;

        if (depth % period == 0)
            severity += depth * range;
    }

#if DAY_13 == 1
    std::cout << severity << std::endl;
#elif DAY_13 == 2
    std::size_t delay = 0;
    while (std::any_of(periods.begin(), periods.end(),
                       [&delay](const std::pair<const std::size_t, std::size_t>& p) {
                           return (p.first + delay) % p.second == 0;
                       }))
        ++delay;
    std::cout << delay << std::endl;
#endif

    return 0;
}

#endif // DAY_13
