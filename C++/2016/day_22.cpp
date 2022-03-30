#ifndef DAY_22
#define DAY_22

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <util/pair_hash.h>

struct search_state {
    std::pair<std::size_t, std::size_t> target_loc;
    std::pair<std::size_t, std::size_t> zero_loc;

    bool operator==(const search_state& rhs) const {
        return target_loc == rhs.target_loc && zero_loc == rhs.zero_loc;
    }

    size_t heuristic() const {
        return target_loc.first + std::abs((long) target_loc.first - (long) zero_loc.first)
             + std::abs((long) target_loc.second - (long) zero_loc.second) - 1;
    }
};

namespace std {
template<> struct hash<search_state> {
    util::pair_hash<std::size_t, std::size_t> pair_hasher;

    size_t operator()(const search_state& s) const {
        return util::asym_combine_hash(pair_hasher(s.target_loc), pair_hasher(s.zero_loc));
    }
};
} // namespace std

int day_22_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    // Part 1
    std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> sizes;
    std::map<std::size_t, std::unordered_set<std::string>> on_used;
    std::map<std::size_t, std::unordered_set<std::string>> on_avail;

    // Part 2
    std::vector<std::vector<bool>> is_wall;
    std::pair<std::size_t, std::size_t> hole_pos;

    std::size_t i = 0;
    for (std::string line; std::getline(reader, line);) {
        if (i < 2) {
            ++i;
            continue;
        }

        std::istringstream line_read(line);

        std::string node;
        line_read >> node;
        node = node.substr(10);
        if (node.substr(node.size() - 2) == "y0")
            is_wall.emplace_back();
        std::size_t x, y, pos1, pos2;
        pos1 = node.find('x');
        pos2 = node.find('y', pos1);
        x = std::stoul(node.substr(pos1 + 1, pos2 - pos1 - 2));
        y = std::stoul(node.substr(pos2 + 1));

        std::string word;
        std::size_t used, avail;
        line_read >> word >> word;
        used = std::stoul(word.substr(0, word.size() - 1));
        line_read >> word;
        avail = std::stoul(word.substr(0, word.size() - 1));

        sizes[node] = std::make_pair(used, avail);
        on_used[used].insert(node);
        on_avail[avail].insert(node);

        is_wall.back().push_back(used > 200);
        if (used == 0)
            hole_pos = std::make_pair(x, y);
    }

    // Part 1
    std::size_t viable_pairs = 0;
    for (const std::pair<const std::size_t, std::unordered_set<std::string>>& u : on_used) {
        if (!u.first)
            continue;
        for (auto it = on_avail.lower_bound(u.first); it != on_avail.end(); ++it) {
            std::size_t num_pairs = it->second.size() * u.second.size();
            for (const std::string& n : u.second)
                if (it->second.count(n))
                    --num_pairs;
            viable_pairs += num_pairs;
        }
    }
    std::cout << viable_pairs << std::endl;

    // Part 2
    std::unordered_map<search_state, std::size_t> dists;
    search_state initial = {std::make_pair(is_wall.size() - 1, 0), hole_pos};
    dists[initial] = 0;
    bool found = false;
    auto compare = [&](const search_state& x, const search_state& y) {
        return dists[x] + x.heuristic() > dists[y] + y.heuristic();
    };
    std::priority_queue<search_state, std::vector<search_state>, decltype(compare)> frontier(
      compare);
    frontier.push(initial);

    while (!found) {
        search_state curr = frontier.top();
        frontier.pop();
        if (curr.target_loc == std::make_pair(0UL, 0UL)) {
            std::cout << dists[curr] << std::endl;
            found = true;
        } else {
            std::size_t curr_dist = dists[curr];

            std::vector<search_state> successors(4, curr);
            --successors[0].zero_loc.first;
            ++successors[1].zero_loc.first;
            --successors[2].zero_loc.second;
            ++successors[3].zero_loc.second;
            for (search_state& s : successors) {
                if (s.zero_loc == curr.target_loc)
                    s.target_loc = curr.zero_loc;
                if (s.zero_loc.first < is_wall.size() && s.zero_loc.second < is_wall.front().size()
                    && !dists.count(s) && !is_wall[s.zero_loc.first][s.zero_loc.second]) {
                    dists[s] = curr_dist + 1;
                    frontier.push(s);
                }
            }
        }
    }

    return 0;
}

#endif // DAY_22
