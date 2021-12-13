#ifndef DAY_1
#define DAY_1 2

#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<char> input(std::istreambuf_iterator<char>{reader},
                            std::istreambuf_iterator<char>{});
    if (input.back() == '\n')
        input.pop_back();

#if DAY_1 == 1
    constexpr std::size_t SHIFT = 1;
#elif DAY_1 == 2
    const std::size_t SHIFT = input.size() / 2;
#endif
    std::copy(input.begin(), input.begin() + SHIFT, std::back_inserter(input));

    std::cout << std::transform_reduce(
                   input.begin(), input.end() - SHIFT, input.begin() + SHIFT, 0UL,
                   std::plus<std::size_t>(),
                   [](char x, char y) -> std::size_t { return (x == y) ? (x - '0') : 0; })
              << std::endl;

    return 0;
}

#endif // DAY_1
