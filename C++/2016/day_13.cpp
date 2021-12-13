#ifndef DAY_13
#define DAY_13 2

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <unordered_set>
#include <vector>

#include "util/pair_hash.h"

bool is_wall(std::size_t favorite, std::size_t x, std::size_t y) {
    std::size_t value = x * x + 3 * x + 2 * x * y + y + y * y + favorite;
    std::size_t hamming = 0;
    while (value != 0) {
        hamming += value % 2;
        value /= 2;
    }
    return hamming % 2;
}

int day_13_main(int argc, char** argv) {
#if DAY_13 == 1
    if (argc < 4)
#elif DAY_13 == 2
    if (argc < 3)
#endif
        return 1;

    constexpr std::size_t START_X(1), START_Y(1);
    std::size_t favorite_number = std::stoul(argv[1]);
#if DAY_13 == 1
    std::size_t target_x = std::stoul(argv[2]);
    std::size_t target_y = std::stoul(argv[3]);
#elif DAY_13 == 2
    std::size_t max_dist = std::stoul(argv[2]);
#endif

#if DAY_13 == 1
    std::unordered_map<std::pair<std::size_t, std::size_t>, std::size_t,
                       util::pair_hash<std::size_t, std::size_t>>
      visited;
#elif DAY_13 == 2
    std::unordered_set<std::pair<std::size_t, std::size_t>,
                       util::pair_hash<std::size_t, std::size_t>>
      visited;
    visited.emplace(START_X, START_Y);
#endif

#if DAY_13 == 1
    std::vector<std::pair<std::size_t, std::pair<std::size_t, std::size_t>>> frontier;
#elif DAY_13 == 2
    std::list<std::pair<std::size_t, std::pair<std::size_t, std::size_t>>> frontier;
#endif
    frontier.emplace_back(0, std::make_pair(START_X, START_Y));

#if DAY_13 == 1
    auto compare =
      [&target_x, &target_y](const std::pair<std::size_t, std::pair<std::size_t, std::size_t>>& x,
                             const std::pair<std::size_t, std::pair<std::size_t, std::size_t>>& y) {
          std::size_t x_heuristic = std::abs((long) x.second.first - (long) target_x)
                                  + std::abs((long) x.second.second - (long) target_y);
          std::size_t y_heuristic = std::abs((long) y.second.first - (long) target_y)
                                  + std::abs((long) y.second.second - (long) target_y);
          return x_heuristic + x.first > y_heuristic + y.first;
      };
#endif

    while (!frontier.empty()) {
#if DAY_13 == 1
        std::pop_heap(frontier.begin(), frontier.end(), compare);
        auto curr = frontier.back();
        frontier.pop_back();
        if (visited.count(curr.second))
            continue;
        visited[curr.second] = curr.first;

        if (curr.second == std::make_pair(target_x, target_y))
            break;
#elif DAY_13 == 2
        auto curr = frontier.front();
        frontier.pop_front();
        if (curr.first == max_dist)
            continue;
#endif

        std::list<std::pair<std::size_t, std::size_t>> neighbors = {
          std::make_pair(curr.second.first + 1, curr.second.second),
          std::make_pair(curr.second.first, curr.second.second + 1)};
        if (curr.second.first)
            neighbors.emplace_back(curr.second.first - 1, curr.second.second);
        if (curr.second.second)
            neighbors.emplace_back(curr.second.first, curr.second.second - 1);

        neighbors.remove_if(
          [&favorite_number, &visited](const std::pair<std::size_t, std::size_t>& p) {
              return visited.count(p) != 0 || is_wall(favorite_number, p.first, p.second);
          });
        for (const std::pair<std::size_t, std::size_t>& p : neighbors) {
            frontier.emplace_back(curr.first + 1, p);
#if DAY_13 == 1
            std::push_heap(frontier.begin(), frontier.end(), compare);
#elif DAY_13 == 2
            visited.insert(p);
#endif
        }
    }

#if DAY_13 == 1
    std::cout << visited[{target_x, target_y}] << std::endl;
#elif DAY_13 == 2
    std::cout << visited.size() << std::endl;
#endif

    return 0;
}

#endif // DAY_13
