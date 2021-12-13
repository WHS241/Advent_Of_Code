#ifndef DAY_9
#define DAY_9

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <set>

int day_9_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t preamble_size = std::stoul(argv[2]);

    std::list<std::size_t> ordered;
    std::set<std::size_t> valids;

    std::size_t first_invalid = -1;

    // Part 1
    auto it = ordered.end();
    for (std::size_t x; reader >> x;) {
        if (ordered.size() >= preamble_size) {
            // Finished preamble, check and remove appropriate values
            if (first_invalid == -1
                && std::none_of(valids.begin(), valids.upper_bound(x),
                                [&x, &valids](const std::size_t y) { return valids.count(x - y); }))
                first_invalid = x;
            valids.erase(*it++);
        }
        valids.insert(x);
        ordered.push_back(x);
        if (ordered.size() == 1)
            it = ordered.begin();
    }
    std::cout << first_invalid << '\n';

    // Part 2
    std::size_t sum = 0;
    it = ordered.begin();
    auto it2 = it;

    // sliding window until sum matches
    while (sum != first_invalid) {
        if (sum < first_invalid)
            sum += *it2++;
        else
            sum -= *it++;
    }
    std::cout << *std::min_element(it, it2) + *std::max_element(it, it2) << std::endl;

    return 0;
}

#endif // DAY_9
