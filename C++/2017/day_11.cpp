#ifndef DAY_11
#define DAY_11 2

#include <cmath>
#include <fstream>
#include <iostream>

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    long x = 0;
    long y = 0;

    auto get_hex_dist = [](long x, long y) -> std::size_t {
        return std::abs(x) > std::abs(y) ? std::abs(x)
                                         : (std::abs(x) + (std::abs(y) - std::abs(x)) / 2);
    };

    std::size_t max_dist = 0;

    for (std::string inst; std::getline(reader, inst, ',');) {
        if (inst == "n")
            y += 2;
        else if (inst == "s")
            y -= 2;
        else {
            if (inst.back() == '\n')
                inst.pop_back();
            if (inst.front() == 's')
                --y;
            else if (inst.front() == 'n')
                ++y;

            if (inst.back() == 'w')
                --x;
            else if (inst.back() == 'e')
                ++x;
        }

        max_dist = std::max(max_dist, get_hex_dist(x, y));
    }

#if DAY_11 == 1
    std::cout << get_hex_dist(x, y) << std::endl;
#elif DAY_11 == 2
    std::cout << max_dist << std::endl;
#endif

    return 0;
}

#endif // DAY_11
