#ifndef DAY_6
#define DAY_6 2

#include <util/pair_hash.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <vector>

namespace std {
template<> struct hash<std::vector<std::size_t>> {
    std::size_t operator()(const std::vector<std::size_t>& v) const {
        return std::accumulate(v.begin(), v.end(), 0UL, [](std::size_t h, std::size_t x) {
            return util::asym_combine_hash(h, std::hash<std::size_t>()(x));
        });
    }
};
} // namespace std

int day_6_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::ifstream reader(argv[1]);

    std::vector<std::size_t> blocks(std::istream_iterator<std::size_t>{reader},
                                    std::istream_iterator<std::size_t>{});

    std::size_t count    = 0;
    std::size_t conflict = 0;
    std::unordered_map<std::vector<std::size_t>, std::size_t> prev;
    prev.emplace(blocks, 0);

    while (count != prev.size()) {
        auto max_it               = std::max_element(blocks.begin(), blocks.end());
        std::size_t to_distribute = *max_it;
        *max_it                   = 0UL;
        for (std::size_t& s : blocks)
            s += (to_distribute - to_distribute % blocks.size()) / blocks.size();
        std::size_t remainder = to_distribute % blocks.size();

        auto it = max_it + 1;
        if (it == blocks.end())
            it = blocks.begin();
        for (std::size_t i = 0; i < remainder; ++i) {
            ++*(it++);
            if (it == blocks.end())
                it = blocks.begin();
        }

        if (!prev.emplace(blocks, ++count).second)
            conflict = prev.at(blocks);
    }

#if DAY_6 == 1
    std::cout << count << std::endl;
#elif DAY_6 == 2
    std::cout << (count - conflict) << std::endl;
#endif
    return 0;
}

#endif // DAY_6
