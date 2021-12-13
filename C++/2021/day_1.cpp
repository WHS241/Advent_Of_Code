#ifndef DAY_1
#define DAY_1

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int day_1_main(int argc, char** argv) {
    if (argc < 3) // AoC 2021: Part1: argv[2] = "1"; Part2: argv[2] = "3"
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::size_t> measurements(std::istream_iterator<std::size_t>{reader},
                                          std::istream_iterator<std::size_t>{});

    /*
    std::cout << "P1: "
              << std::inner_product(measurements.begin(), measurements.end() - 1,
                                    measurements.begin() + 1, std::size_t(0), std::plus<>(),
                                    std::less<>())
              << std::endl;

    if (argc < 3) // AoC 2021 D1: argv[2] = "3"
        return 0;
    */

    std::size_t window = std::stoul(argv[2]);
    if (window > measurements.size()) {
        std::cout << "OVERSIZE" << std::endl;
        return 1;
    }

    /*
    std::vector<std::size_t> window_sums(measurements.size() - window + 1);
    for (std::size_t i = 0; i < window_sums.size(); ++i)
        window_sums[i] = std::reduce(measurements.cbegin() + i, measurements.cbegin() + i + window);

    std::cout << "P2: " << std::transform_reduce(window_sums.begin(), window_sums.end() - 1,
                                       window_sums.begin() + 1, std::size_t(0), std::plus<>(),
                                       std::less<>())
              << std::endl;
    */

    // measurements[0] + ... + measurements[n - 1] < measurements[1] + ... + measurements[n]
    //     iff measurements[0] < measurements[n]
    std::cout << std::transform_reduce(measurements.begin(), measurements.end() - window,
                                       measurements.begin() + window, std::size_t(0), std::plus<>(),
                                       std::less<>())
              << std::endl;
    return 0;
}

#endif // DAY_1
