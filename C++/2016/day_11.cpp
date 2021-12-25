#ifndef DAY_11
#define DAY_11

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "util/pair_hash.h"

constexpr std::size_t NUM_FLOORS = 4;

struct state {
    std::multiset<std::pair<std::size_t, std::size_t>> locations; // 顺序: chip, generator
    std::size_t current_floor;

    bool operator==(const state& rhs) const {
        return locations == rhs.locations && current_floor == rhs.current_floor;
    }
};

namespace std {
template<> struct hash<state> {
    std::size_t operator()(const state& s) const {
        std::hash<std::size_t> hasher;
        std::size_t value = hasher(s.current_floor);
        util::pair_hash<std::size_t, std::size_t> loc_hasher;
        for (const std::pair<std::size_t, std::size_t>& p : s.locations)
            value = util::asym_combine_hash(value, loc_hasher(p));
        return value;
    }
};
} // namespace std

bool is_valid(const state& s) {
    std::array<std::size_t, NUM_FLOORS> gens_per_floor;
    std::fill(gens_per_floor.begin(), gens_per_floor.end(), 0UL);
    for (const std::pair<std::size_t, std::size_t>& p : s.locations)
        ++gens_per_floor[p.second];
    return std::all_of(s.locations.begin(), s.locations.end(),
                       [&gens_per_floor](const std::pair<std::size_t, std::size_t>& p) {
                           return p.first == p.second || !gens_per_floor[p.first];
                       });
}

bool is_final(const state& s) {
    return s.current_floor + 1 == NUM_FLOORS
        && std::all_of(s.locations.begin(), s.locations.end(),
                       [](const std::pair<std::size_t, std::size_t>& p) {
                           return p.first == p.second && p.first == NUM_FLOORS - 1;
                       });
}

std::unordered_set<state> get_successors(const state& prev) {
    std::unordered_set<state> result;
    if (is_final(prev) || !is_valid(prev))
        return result;

    bool can_move_up(prev.current_floor != NUM_FLOORS - 1), can_move_down(prev.current_floor);
    std::vector<std::size_t> targets;
    targets.reserve(2);
    if (can_move_up)
        targets.push_back(prev.current_floor + 1);
    if (can_move_down)
        targets.push_back(prev.current_floor - 1);
    for (std::size_t dest : targets) {
        state next(prev);
        next.current_floor = dest;
        for (auto it = prev.locations.begin(); it != prev.locations.end(); ++it) {
            if (it->second == prev.current_floor) {
                // 一个发电机
                auto inserted = next.locations.emplace(it->first, dest);
                next.locations.erase(next.locations.find(*it));
                result.insert(next);

                // 两个发电机
                auto it2 = it;
                for (++it2; it2 != prev.locations.end(); ++it2) {
                    if (it2->second == prev.current_floor) {
                        auto inserted2 = next.locations.emplace(it2->first, dest);
                        next.locations.erase(next.locations.find(*it2));
                        result.insert(next);
                        next.locations.insert(*it2);
                        next.locations.erase(inserted2);
                    }
                }

                next.locations.insert(*it);
                next.locations.erase(inserted);
            }
        }

        for (auto it = prev.locations.begin(); it != prev.locations.end(); ++it) {
            if (it->first == prev.current_floor) {
                // 一个芯片
                auto inserted = next.locations.emplace(dest, it->second);
                next.locations.erase(next.locations.find(*it));
                result.insert(next);

                bool passed_first = false;
                for (auto it2 = prev.locations.begin(); it2 != prev.locations.end(); ++it2) {
                    if (it2 == it) {
                        passed_first = true;

                        // 芯片加相应的发电机
                        if (it->first == it->second) {
                            next.locations.erase(inserted);
                            inserted = next.locations.emplace(dest, dest);
                            result.insert(next);
                            next.locations.erase(inserted);
                            inserted = next.locations.emplace(dest, dest);
                        }
                    } else if (passed_first && it2->first == prev.current_floor) {
                        // 不能运一个芯片和另一类的发电机，肯定会爆炸

                        // 两个芯片
                        auto inserted2 = next.locations.emplace(dest, it2->second);
                        next.locations.erase(next.locations.find(*it2));
                        result.insert(next);
                        next.locations.insert(*it2);
                        next.locations.erase(inserted2);
                    }
                }

                next.locations.insert(*it);
                next.locations.erase(inserted);
            }
        }
    }

    for (auto it = result.begin(); it != result.end();)
        if (is_valid(*it))
            ++it;
        else
            it = result.erase(it);

    return result;
}

std::size_t get_heuristic(const state& s) {
    return std::transform_reduce(s.locations.begin(), s.locations.end(), 0UL, std::plus<>(),
                                 [](const std::pair<std::size_t, std::size_t>& p) {
                                     return 2 * NUM_FLOORS - (p.first + p.second + 2);
                                 })
         / 2;
}

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t current_floor = 0;

    std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> initial_locations;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string word;
        line_read >> word >> word >> word >> word;
        while (line_read >> word) {
            if (word == "nothing")
                break;
            if (word == "and")
                line_read >> word;
            std::string element, type;
            line_read >> element >> type;
            bool is_chip = type.front() == 'm';
            if (is_chip)
                element.resize(element.find_first_of('-'));
            std::size_t& to_set =
              is_chip ? initial_locations[element].first : initial_locations[element].second;
            to_set = current_floor;
        }
        ++current_floor;
    }

    auto compare = [](const std::pair<state, std::size_t>& x,
                      const std::pair<state, std::size_t>& y) {
        return get_heuristic(x.first) + x.second > get_heuristic(y.first) + y.second;
    };

    state initial_state;
    initial_state.current_floor = 0;
    for (auto& p : initial_locations)
        initial_state.locations.insert(p.second);

    std::vector<std::pair<state, std::size_t>> frontier;
    frontier.emplace_back(initial_state, 0);

    std::unordered_map<state, std::size_t> visited;

    while (!frontier.empty()) {
        std::pop_heap(frontier.begin(), frontier.end(), compare);
        std::pair<state, std::size_t> curr = frontier.back();
        frontier.pop_back();
        if (is_final(curr.first)) {
            std::cout << curr.second << std::endl;
            break;
        }
        if (!visited.count(curr.first) || visited[curr.first] >= curr.second) {
            visited[curr.first] = curr.second;
            for (const state& s : get_successors(curr.first)) {
                if (!visited.count(s) || visited[s] > curr.second + 1) {
                    visited[s] = curr.second + 1;
                    frontier.emplace_back(s, curr.second + 1);
                    std::push_heap(frontier.begin(), frontier.end(), compare);
                }
            }
        }
    }

    return 0;
}

#endif // DAY_11
