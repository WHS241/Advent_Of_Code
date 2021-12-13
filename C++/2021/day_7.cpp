#ifndef DAY_7
#define DAY_7

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "sequence/order_stats.h"

int day_7_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<long> initial;

    for (std::string val; std::getline(reader, val, ',');)
        initial.push_back(std::stoul(val));

    // P1: must be median, can prove by induction on list size (IH: k-2 holds)
    long median = sequence::selection(initial.begin(), initial.end(), initial.size() / 2);

    // P2: sum(d(d + 1)/2) = sum((m - x)^2/2 + |m - x|/2)
    // Minimized at d/dm = 0, or sum(m - x + sgn(m - x)/2) = 0
    // -n / 2 <= sum(m - x) <= n / 2
    // -1/2 <= m - mean(x) <= 1/2
    std::vector<long> p2_dists(initial.size());
    for (long i = 0; i < (long) initial.size(); ++i) {
        for (long j = 0; j < (long) p2_dists.size(); ++j) {
            long distance = std::abs(initial[i] - j);
            p2_dists[j] += distance * (distance + 1) / 2;
        }
    }

    std::cout << std::transform_reduce(initial.begin(), initial.end(), 0L, std::plus<>(),
                                       [&median](const long& l) { return std::abs(l - median); })
              << '\n'
              << *std::min_element(p2_dists.begin(), p2_dists.end()) << std::endl;

    return 0;
}

#endif // DAY_7
