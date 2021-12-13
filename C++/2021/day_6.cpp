#ifndef DAY_6
#define DAY_6

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>

int day_6_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_days = std::stoul(argv[2]);

    std::array<std::size_t, 9> num_at_age;
    std::fill(num_at_age.begin(), num_at_age.end(), 0UL);
    for (std::string val; std::getline(reader, val, ',');)
        ++num_at_age[std::stoul(val)];

    for (std::size_t i = 0; i < num_days; ++i) {
        std::rotate(num_at_age.begin(), num_at_age.begin() + 1, num_at_age.begin() + 7);
        std::rotate(num_at_age.begin() + 6, num_at_age.begin() + 7, num_at_age.end());
        num_at_age[6] += num_at_age[8];
    }

    std::cout << std::reduce(num_at_age.begin(), num_at_age.end()) << std::endl;

    return 0;
}

#endif // DAY_6
