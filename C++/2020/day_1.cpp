#ifndef DAY_1
#define DAY_1

#include <fstream>
#include <iostream>
#include <unordered_set>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    constexpr std::size_t target = 2020;

    std::size_t part_1(0), part_2(0);

    std::unordered_set<std::size_t> encountered;
    for (std::string line; std::getline(reader, line);) {
        std::size_t x = std::stoul(line);
        if (encountered.count(target - x))
            part_1 = x * (target - x);
        for (std::size_t y : encountered) {
            if (encountered.count(target - x - y)) {
                part_2 = x * y * (target - x - y);
                break;
            }
        }
        encountered.insert(x);
    }

    std::cout << part_1 << '\n' << part_2 << std::endl;

    return 0;
}

#endif
