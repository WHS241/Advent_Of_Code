#ifndef DAY_6
#define DAY_6 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <numeric>
#include <utility>
#include <vector>

int day_6_main(int argc, char** argv) {
#if DAY_6 == 1
    if (argc < 2)
#elif DAY_6 == 2
    if (argc < 3)
#endif
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::pair<uint16_t, uint16_t>> points;
    uint16_t minX(std::numeric_limits<uint16_t>::max()), maxX(0);
    uint16_t minY(minX), maxY(0);

    for (std::string line; std::getline(reader, line);) {
        std::size_t pos = line.find(',');
        uint16_t x(std::stoi(line.substr(0, pos))), y(std::stoi(line.substr(pos + 1)));
        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);

        points.emplace_back(x, y);
    }

#if DAY_6 == 1
    std::vector<std::vector<std::pair<uint16_t, std::size_t>>> grid(
      maxX - minX + 1,
      std::vector<std::pair<uint16_t, std::size_t>>(maxY - minY + 1, {-1, points.size()}));

    for (std::size_t i = 0; i < points.size(); ++i) {
        std::pair<uint16_t, uint16_t> p = points[i];
        std::pair<uint16_t, uint16_t> t(p.first - minX, p.second - minY);
        std::list<std::pair<uint16_t, std::pair<uint16_t, uint16_t>>> frontier;
        frontier.emplace_back(0, t);
        grid[t.first][t.second] = {0, i};

        while (!frontier.empty()) {
            auto s = frontier.front();
            frontier.pop_front();

            std::array<std::pair<uint16_t, uint16_t>, 4> neighbors{
              std::make_pair(s.second.first - 1, s.second.second),
              std::make_pair(s.second.first + 1, s.second.second),
              std::make_pair(s.second.first, s.second.second - 1),
              std::make_pair(s.second.first, s.second.second + 1)};

            for (auto p : neighbors) {
                if (p.first < grid.size() && p.second < grid[0].size()) {
                    if (s.first + 1 < grid[p.first][p.second].first) {
                        grid[p.first][p.second] = {s.first + 1, i};
                        frontier.emplace_back(s.first + 1, p);
                    } else if (s.first + 1 == grid[p.first][p.second].first
                               && i != grid[p.first][p.second].second)
                        grid[p.first][p.second].second = points.size();
                }
            }
        }
    }

    std::vector<bool> ignored(points.size() + 1, false);
    ignored[points.size()] = true;

    for (std::size_t i = 0; i < grid.size(); ++i)
        ignored[grid[i][0].second] = ignored[grid[i].back().second] = true;

    for (std::size_t i = 0; i < grid[0].size(); ++i)
        ignored[grid[0][i].second] = ignored[grid.back().front().second] = true;

    std::vector<std::size_t> areas(points.size(), 0);
    for (auto& r : grid)
        for (auto& p : r)
            if (!ignored[p.second])
                ++areas[p.second];

    std::cout << *std::max_element(areas.begin(), areas.end()) << std::endl;
#elif DAY_6 == 2
    long limit = std::stol(argv[2]);

    std::size_t count = 0;
    for (long x = long(minX) - limit; x <= long(maxX) + limit; ++x) {
        bool exceeded = true;
        for (long y = long(minY) - limit; y <= long(maxY) + limit; ++y) {
            std::size_t total_dist = std::transform_reduce(
              points.begin(), points.end(), 0UL, std::plus<>(),
              [&x, &y](const std::pair<uint16_t, uint16_t>& p) {
                  return std::abs(long(p.first) - x) + std::abs(long(p.second) - y);
              });
            if (total_dist < limit) {
                exceeded = false;
                ++count;
            } else if (y >= maxY)
                break;
        }
        if (x >= maxX && exceeded)
            break;
    }

    std::cout << count << std::endl;
#endif

    return 0;
}

#endif // DAY_6
