#ifndef DAY_18
#define DAY_18

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int day_18_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_rows = std::stoul(argv[2]);

    std::vector<bool> is_trap;
    is_trap.push_back(false);

    for (char c; reader >> c;)
        is_trap.push_back(c == '^');

    is_trap.push_back(false);
    std::size_t num_safe = std::count(is_trap.begin() + 1, is_trap.end() - 1, false);

    for (std::size_t i = 1; i < num_rows; ++i) {
        std::vector<bool> next(is_trap.size());
        next[0] = false;
        next[next.size() - 1] = false;
        for (std::size_t j = 1; j < next.size() - 1; ++j)
            next[j] = is_trap[j - 1] ^ is_trap[j + 1];
        num_safe += std::count(next.begin() + 1, next.end() - 1, false);
        is_trap = std::move(next);
    }

    std::cout << num_safe << std::endl;

    return 0;
}

#endif // DAY_18
