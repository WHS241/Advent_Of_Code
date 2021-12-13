#ifndef DAY_17
#define DAY_17

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <util/pair_hash.h>

namespace std {
template<> struct hash<std::vector<int>> {
    std::size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        return std::accumulate(v.begin(), v.end(), std::size_t(0), [&](std::size_t acc, int x) {
            return util::asym_combine_hash(acc, hasher(x));
        });
    }
};
} // namespace std

std::list<std::vector<int>> get_neighbors(const std::vector<int>& pos) {
    std::list<std::vector<int>> result;
    std::vector<int> curr(pos.size());

    std::transform(pos.begin(), pos.end(), curr.begin(), [](int i) { return i - 1; });

    bool finished = false;
    while (!finished) {
        result.push_back(curr);

        finished = true;
        for (std::size_t i = 0; i < pos.size(); ++i) {
            ++curr[i];
            if (curr[i] > pos[i] + 1) {
                curr[i] = pos[i] - 1;
            } else {
                finished = false;
                break;
            }
        }
    }

    result.remove(pos);
    return result;
}

int day_17_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    constexpr char ALIVE('#');
    constexpr std::size_t CYCLES = 6;
    std::ifstream reader(argv[1]);
    std::size_t dimensions = std::stoul(argv[2]);

    if (dimensions < 2)
        throw std::invalid_argument("dims < 2");

    std::vector<int> pos(dimensions, 0);
    std::unordered_set<std::vector<int>> alive_cells;

    for (std::string line; std::getline(reader, line);) {
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] == ALIVE) {
                pos[1] = i;
                alive_cells.insert(pos);
            }
        }

        ++pos[0];
    }

    for (std::size_t i = 0; i < CYCLES; ++i) {
        auto next(alive_cells);
        std::unordered_map<std::vector<int>, std::size_t> counts;

        for (const auto& p : alive_cells)
            for (const auto& n : get_neighbors(p))
                ++counts[n];

        std::erase_if(
          next, [&counts](const std::vector<int>& p) { return counts[p] != 2 && counts[p] != 3; });
        for (const auto& x : counts)
            if (!alive_cells.contains(x.first) && x.second == 3)
                next.insert(x.first);

        alive_cells = std::move(next);
    }

    std::cout << alive_cells.size() << std::endl;
    return 0;
}

#endif // DAY_17
