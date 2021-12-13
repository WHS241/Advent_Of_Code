#ifndef DAY_10
#define DAY_10 2

#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

constexpr std::size_t MAX_DIFF = 3;

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::set<std::size_t> adapters(std::istream_iterator<std::size_t>{reader},
                                   std::istream_iterator<std::size_t>{});

    // Part 1
    std::array<std::size_t, MAX_DIFF> num_differences;
    std::fill(num_differences.begin(), num_differences.end(), 0);
    ++*num_differences.rbegin();              // always 3 jolts between highest and device
    ++num_differences[*adapters.begin() - 1]; // connect to 0-jolt outlet
    for (auto it = adapters.begin(); it != adapters.end(); ++it) {
        auto it2 = it;
        ++it2;
        if (it2 != adapters.end())
            ++num_differences[*it2 - *it - 1];
    }
    std::cout << num_differences[0] * num_differences[2] << '\n';

    // Part 2
    std::map<std::size_t, std::size_t> counts;
    counts[0] = 1;
    for (auto p : counts)
        for (auto it = adapters.upper_bound(p.first);
             it != adapters.upper_bound(p.first + MAX_DIFF); ++it)
            counts[*it] += p.second;
    std::cout << counts.rbegin()->second << std::endl;

    return 0;
}

#endif // DAY_10
