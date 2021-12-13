#ifndef DAY_3
#define DAY_3

#define DAY_3_PART 2

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct segment {
    std::pair<int, int> endpoint1;
    std::pair<int, int> endpoint2;
};

// 根据input把所有的电线部分分成横竖两组
// 同时按照顺序排放到order里
// order: [ { segment (从左到右或从上到下），电流是不是反方向，是不是横的线 } ]
template<typename T, typename U>
void populate_segments(std::string input, std::set<segment, T>& horizontal,
                       std::set<segment, U>& vertical,
                       std::vector<std::tuple<segment, bool, bool>>& order) {
    std::istringstream reader(input);
    int start_x = 0, start_y = 0;

    for (std::string instruction; std::getline(reader, instruction, ',');) {
        char direction = instruction[0];
        int distance = std::stoi(instruction.substr(1));
        segment next;
        next.endpoint1 = {start_x, start_y};
        bool reversed(false), is_horizontal;
        switch (direction) {
        case 'U':
            start_y += distance;
            next.endpoint2 = {start_x, start_y};
            break;

        case 'D':
            next.endpoint2 = next.endpoint1;
            start_y -= distance;
            next.endpoint1 = {start_x, start_y};
            reversed = true;
            break;

        case 'R':
            start_x += distance;
            next.endpoint2 = {start_x, start_y};
            break;

        case 'L':
            start_x -= distance;
            next.endpoint2 = next.endpoint1;
            next.endpoint1 = {start_x, start_y};
            reversed = true;
            break;

        default:
            break;
        }

        switch (direction) {
        case 'U':
        case 'D':
            vertical.insert(next);
            is_horizontal = false;
            break;

        case 'L':
        case 'R':
            horizontal.insert(next);
            is_horizontal = true;
            break;

        default:
            break;
        }

        order.emplace_back(next, reversed, is_horizontal);
    }
}

struct order_horizontal {
    bool operator()(const segment& x, const segment& y) const {
        return x.endpoint1.second < y.endpoint1.second
            || (x.endpoint1.second == y.endpoint1.second && x.endpoint1.first < y.endpoint1.first);
    }
};

struct order_vertical {
    bool operator()(const segment& x, const segment& y) const {
        return x.endpoint1.first < y.endpoint1.first
            || (x.endpoint1.first == y.endpoint1.first && x.endpoint1.second < y.endpoint1.second);
    }
};

int day_3_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream in_stream(argv[1]);

    std::string curr_line;
    std::getline(in_stream, curr_line);

    std::set<segment, order_horizontal> wire1_hor, wire2_hor;
    std::set<segment, order_vertical> wire1_vert, wire2_vert;

    std::vector<std::tuple<segment, bool, bool>> wire1, wire2;

    populate_segments(curr_line, wire1_hor, wire1_vert, wire1);

    std::getline(in_stream, curr_line);
    populate_segments(curr_line, wire2_hor, wire2_vert, wire2);

    std::map<int, std::set<int>> intersections_on_x, intersections_on_y;

    // 把两根线之间所有的交叉点都找出来，现在顺序并不重要
    // wire1_hor & wire2_vert
    for (const segment& s : wire1_hor) {
        segment lower;
        lower.endpoint1 = {s.endpoint1.first, INT_MIN};
        for (auto it = wire2_vert.lower_bound(lower); it != wire2_vert.end(); ++it) {
            const segment& s2 = *it;
            if (s2.endpoint1.first > s.endpoint2.first)
                break;
            if (s2.endpoint1.second <= s.endpoint1.second
                && s.endpoint1.second <= s2.endpoint2.second) {
                int dist = std::abs(s.endpoint1.second) + std::abs(s2.endpoint1.first);
                if (dist != 0) {
                    intersections_on_x[s2.endpoint1.first].insert(s.endpoint1.second);
                    intersections_on_y[s.endpoint1.second].insert(s2.endpoint1.first);
                }
            }
        }
    }

    // wire1_vert and wire2_hor
    for (const segment& s : wire1_vert) {
        segment lower;
        lower.endpoint1 = {INT_MIN, s.endpoint1.second};
        for (auto it = wire2_hor.lower_bound(lower); it != wire2_hor.end(); ++it) {
            const segment& s2 = *it;
            if (s2.endpoint1.second > s.endpoint2.second)
                break;
            if (s2.endpoint1.first <= s.endpoint1.first
                && s.endpoint1.first <= s2.endpoint2.first) {
                int dist = std::abs(s.endpoint1.first) + std::abs(s2.endpoint1.second);
                if (dist != 0) {
                    intersections_on_x[s.endpoint1.first].insert(s2.endpoint1.second);
                    intersections_on_y[s2.endpoint2.second].insert(s.endpoint1.first);
                }
            }
        }
    }

#if DAY_3_PART == 1
    std::size_t closest_dist = ULONG_MAX;
    for (const std::pair<const int, std::set<int>>& p : intersections_on_x)
        for (int p2 : p.second)
            closest_dist = std::min(closest_dist, std::abs(p.first) + std::abs(p2));
    std::cout << closest_dist << std::endl;
#elif DAY_3_PART == 2
    int total_delay = 0;
    std::unordered_map<int, std::unordered_map<int, int>> delays;
    std::unordered_map<int, std::unordered_set<int>> visited;

    // 计算第一根电线对于所有交叉点的delay
    for (const std::tuple<segment, bool, bool>& s : wire1) {
        segment curr;
        bool reversed;
        bool horizontal;
        std::tie(curr, reversed, horizontal) = s;

        if (horizontal) {
            if (intersections_on_y.find(curr.endpoint1.second) != intersections_on_y.end()) {
                const std::set<int>& to_check = intersections_on_y[curr.endpoint1.second];
                for (auto it = to_check.lower_bound(curr.endpoint1.first);
                     it != to_check.end() && *it <= curr.endpoint2.first; ++it) {
                    if (visited[*it].find(curr.endpoint1.second) == visited[*it].end()) {
                        visited[*it].insert(curr.endpoint1.second);
                        delays[*it][curr.endpoint1.second] =
                          total_delay
                          + (reversed ? curr.endpoint2.first - *it : *it - curr.endpoint1.first);
                    }
                }
            }
            total_delay += curr.endpoint2.first - curr.endpoint1.first;
        } else {
            if (intersections_on_x.find(curr.endpoint1.first) != intersections_on_x.end()) {
                const std::set<int>& to_check = intersections_on_x[curr.endpoint1.first];
                for (auto it = to_check.lower_bound(curr.endpoint1.second);
                     it != to_check.end() && *it <= curr.endpoint2.second; ++it) {
                    if (visited[curr.endpoint1.first].find(*it)
                        == visited[curr.endpoint1.first].end()) {
                        visited[curr.endpoint1.first].insert(*it);
                        delays[curr.endpoint1.first][*it] =
                          total_delay
                          + (reversed ? curr.endpoint2.second - *it : *it - curr.endpoint1.second);
                    }
                }
            }
            total_delay += curr.endpoint2.second - curr.endpoint1.second;
        }
    }

    // 第二根
    total_delay = 0;
    visited.clear();
    for (const std::tuple<segment, bool, bool>& s : wire2) {
        segment curr;
        bool reversed;
        bool horizontal;
        std::tie(curr, reversed, horizontal) = s;

        if (horizontal) {
            if (intersections_on_y.find(curr.endpoint1.second) != intersections_on_y.end()) {
                const std::set<int>& to_check = intersections_on_y[curr.endpoint1.second];
                for (auto it = to_check.lower_bound(curr.endpoint1.first);
                     it != to_check.end() && *it <= curr.endpoint2.first; ++it) {
                    if (visited[*it].find(curr.endpoint1.second) == visited[*it].end()) {
                        visited[*it].insert(curr.endpoint1.second);
                        delays[*it][curr.endpoint1.second] +=
                          total_delay
                          + (reversed ? curr.endpoint2.first - *it : *it - curr.endpoint1.first);
                    }
                }
            }
            total_delay += curr.endpoint2.first - curr.endpoint1.first;
        } else {
            if (!horizontal
                && intersections_on_x.find(curr.endpoint1.first) != intersections_on_x.end()) {
                const std::set<int>& to_check = intersections_on_x[curr.endpoint1.first];
                for (auto it = to_check.lower_bound(curr.endpoint1.second);
                     it != to_check.end() && *it <= curr.endpoint2.second; ++it) {
                    if (visited[curr.endpoint1.first].find(*it)
                        == visited[curr.endpoint1.first].end()) {
                        visited[curr.endpoint1.first].insert(*it);
                        delays[curr.endpoint1.first][*it] +=
                          total_delay
                          + (reversed ? curr.endpoint2.second - *it : *it - curr.endpoint1.second);
                    }
                }
            }
            total_delay += curr.endpoint2.second - curr.endpoint1.second;
        }
    }

    int lowest_delay = INT_MAX;
    for (std::pair<const int, std::unordered_map<int, int>>& level_1 : delays) {
        for (std::pair<const int, int>& values : level_1.second) {
            lowest_delay = std::min(lowest_delay, values.second);
        }
    }

    std::cout << lowest_delay << std::endl;
#endif

    return 0;
}

#endif // DAY_3
