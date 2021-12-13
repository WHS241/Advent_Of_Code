#ifndef DAY_17
#define DAY_17

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <vector>

bool advance(std::vector<bool>& v) {
    if (std::all_of(v.begin(), v.end(), [](bool v) { return v; }))
        return false;
    for (auto it = v.begin(); it != v.end(); ++it) {
        *it = !*it;
        if (*it)
            break;
    }
    return true;
}

int day_17_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<std::size_t> containers;
    std::size_t target = std::stoul(argv[2]);

    for (std::size_t i; reader >> i;)
        containers.push_back(i);

    std::vector<bool> pick(containers.size(), false);

    std::size_t count = 0;
    std::vector<std::size_t> num_possibilities(containers.size(), 0);

    while (advance(pick)) {
        std::size_t total = 0;
        std::size_t num = 0;
        for (std::size_t i = 0; i < containers.size() && total <= target; ++i) {
            if (pick[i]) {
                total += containers[i];
                ++num;
            }
        }

        if (total == target) {
            ++count;
            ++num_possibilities[num];
        }
    }

    std::cout << count << std::endl
              << *std::find_if(num_possibilities.begin(), num_possibilities.end(),
                               [](std::size_t x) { return x != 0; })
              << std::endl;

    return 0;
}

#endif
