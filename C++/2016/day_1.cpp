#ifndef DAY_1
#define DAY_1

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::size_t direction = 0;

    std::array<std::pair<int, int>, 4> vectors = {std::make_pair(0, 1), std::make_pair(1, 0),
                                                  std::make_pair(0, -1), std::make_pair(-1, 0)};

    std::ifstream reader(argv[1]);

    std::pair<int, int> current = {0, 0};
    std::pair<int, int> intersect_coords = {0, 0};

    std::map<int, std::map<int, int>> horizontals, verticals; // {fixed, {endpoint1 <-> encpoint2}}
    for (std::string instr; reader >> instr;) {
        if (instr.back() == ',')
            instr.pop_back();
        direction = (direction + (instr.front() == 'R' ? 1 : 3)) % 4;
        int dist = std::stoi(instr.substr(1));

        if (direction % 2 == 0) {
            // Vertical
            int intersection = current.second + dist * vectors[direction].second;
            auto pair1 = verticals[current.first].emplace(current.second, intersection);
            auto pair2 = verticals[current.first].emplace(intersection, current.second);

            // Up or down
            if (direction == 0)
                ++pair1.first;
            else
                --pair1.first;

            // overlap with another in similar direction segment
            if (pair1.first != pair2.first)
                intersection = pair1.first->first;

            // overlap with perpendicular
            if (direction == 0) {
                for (auto it = horizontals.upper_bound(current.second);
                     it != horizontals.upper_bound(intersection); ++it) {
                    auto it2 = it->second.lower_bound(current.first);
                    if (it2 != it->second.end() && it2->second <= current.first) {
                        intersection = it->first;
                        break;
                    }
                }
            } else {
                for (auto it = horizontals.lower_bound(intersection);
                     it != horizontals.lower_bound(current.second); ++it) {
                    auto it2 = it->second.lower_bound(current.first);
                    if (it2 != it->second.end() && it2->second <= current.first)
                        intersection = it->first;
                }
            }

            if (intersect_coords == std::make_pair(0, 0)
                && (std::abs(intersection - current.second) != dist || !pair2.second)) {
                intersect_coords = {current.first, intersection};
            }
            current.second += dist * vectors[direction].second;
        } else {
            int intersection = current.first + dist * vectors[direction].first;
            auto pair1 = horizontals[current.second].emplace(current.first, intersection);
            auto pair2 = horizontals[current.second].emplace(intersection, current.first);
            if (direction == 1)
                ++pair1.first;
            else
                --pair1.first;
            if (pair1.first != pair2.first)
                intersection = pair1.first->first;
            if (direction == 1) {
                for (auto it = verticals.upper_bound(current.first);
                     it != verticals.upper_bound(intersection); ++it) {
                    auto it2 = it->second.lower_bound(current.second);
                    if (it2 != it->second.end() && it2->second <= current.second) {
                        intersection = it->first;
                        break;
                    }
                }
            } else {
                for (auto it = verticals.lower_bound(intersection);
                     it != verticals.lower_bound(current.first); ++it) {
                    auto it2 = it->second.lower_bound(current.second);
                    if (it2 != it->second.end() && it2->second <= current.second)
                        intersection = it->first;
                }
            }

            if (intersect_coords == std::make_pair(0, 0)
                && (std::abs(intersection - current.first) != dist || !pair2.second)) {
                intersect_coords = {intersection, current.second};
            }
            current.first += dist * vectors[direction].first;
        }
    }
    std::cout << std::abs(current.first) + std::abs(current.second) << '\n'
              << std::abs(intersect_coords.first) + std::abs(intersect_coords.second) << std::endl;

    return 0;
}

#endif // DAY_1
