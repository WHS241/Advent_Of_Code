#ifndef DAY_19
#define DAY_19 2

#include <iostream>
#include <string>

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::size_t num_elves = std::stoul(argv[1]);

#if DAY_19 == 1
    // Equivalent to Josephus problem, k = 2
    std::size_t leading_binary = 1;
    while (2 * leading_binary < num_elves)
        leading_binary *= 2;
    std::cout << 2 * (num_elves - leading_binary) + 1 << std::endl;
#elif DAY_19 == 2
    std::size_t curr = 0; // for modular arithmetic
    for (std::size_t i = 2; i <= num_elves; ++i) {
        if (i / 2 == 1 || curr + 1 >= i / 2)
            ++curr;
        ++curr;
        curr %= i;
    }

    std::cout << curr + 1 << std::endl;
#endif

    return 0;
}

#endif // DAY_19
