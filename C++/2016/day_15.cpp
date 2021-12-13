#ifndef DAY_15
#define DAY_15

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

int day_15_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::pair<std::size_t, std::size_t>> discs;

    for (std::string line; std::getline(reader, line);) {
        discs.emplace_back();
        std::string temp;
        std::istringstream line_read(line);

        line_read >> temp >> temp >> temp >> temp;
        discs.back().first = std::stoul(temp);
        line_read >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> temp;
        discs.back().second = std::stoul(temp);
    }

    std::vector<std::size_t> mods(discs.size());
    for (std::size_t i = 0; i < discs.size(); ++i)
        mods[i] = (3 * discs[i].first - discs[i].second - i - 1) % discs[i].first;

    std::size_t time = 0;
    std::size_t increment = 1;
    for (std::size_t i = 0; i < discs.size(); ++i) {
        while (time % discs[i].first != mods[i])
            time += increment;
        increment = std::lcm(increment, discs[i].first);
    }

    std::cout << time << std::endl;

    return 0;
}

#endif // DAY_15
