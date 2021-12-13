#ifndef DAY_4
#define DAY_4

#include <algorithm>
#include <iostream>
#include <string>

int day_4_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::size_t lower = std::stoul(argv[1]);
    std::size_t upper = std::stoul(argv[2]);

    std::size_t count_1(0), count_2(0);

    for (std::size_t i = lower; i <= upper; ++i) {
        std::string rep = std::to_string(i);
        std::size_t counts[10];
        std::fill_n(counts, 10, 0);
        bool increasing = true;
        for (std::size_t j = 0; increasing && j < rep.size(); ++j) {
            if (j != 0 && rep[j - 1] > rep[j]) {
                increasing = false;
            } else {
                ++counts[rep[j] - '0'];
            }
        }

        if (increasing) {
            if (std::find_if(counts, counts + 10, [](std::size_t i) { return i >= 2; }) != counts + 10)
                ++count_1;
            if (std::find(counts, counts + 10, 2) != counts + 10)
                ++count_2;
        }
    }

    std::cout << count_1 << '\n' << count_2 << std::endl;
    return 0;
}

#endif // DAY_4
