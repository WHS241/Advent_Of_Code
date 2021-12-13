#ifndef DAY_13
#define DAY_13 2

#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::string line;
    std::getline(reader, line);

    std::size_t start = std::stoul(line);
    std::vector<std::string> durations;

    for (std::string val; std::getline(reader, val, ',');)
        durations.push_back(std::move(val));

    // Part 1
    std::size_t min_wait = std::numeric_limits<std::size_t>::max();
    std::size_t id;

    for (const std::string& val : durations) {
        if (!std::isdigit(val.front()))
            continue;

        std::size_t i = std::stoul(val);
        min_wait = std::min(min_wait, (i - start % i) % i);
        if ((start + min_wait) % i == 0)
            id = i;
    }
    std::cout << min_wait * id << '\n';

    // Part 2
    std::size_t curr = 1;
    std::size_t increment = 1; // this increment keeps time-to-previous-buses invariant

    for (std::size_t i = 0; i < durations.size(); ++i) {
        if (std::isdigit(durations[i].front())) {
            std::size_t bus = std::stoul(durations[i]);

            while ((curr + i) % bus != 0)
                curr += increment;

            increment = std::lcm(increment, bus);
        }
    }

    std::cout << curr << std::endl;

    return 0;
}

#endif // DAY_13
