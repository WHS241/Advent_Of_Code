#ifndef DAY_11
#define DAY_11 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    constexpr char FLOOR('.'), EMPTY('L'), FULL('#');
#if DAY_11 == 1
    constexpr std::size_t THRESHOLD = 4;
#elif DAY_11 == 2
    constexpr std::size_t THRESHOLD = 5;
#endif

    std::ifstream reader(argv[1]);
    std::vector<std::string> grid;

    for (std::string line; std::getline(reader, line);)
        grid.push_back(line);

    const std::array<std::pair<long, long>, 8> trajectories = {
      std::make_pair(-1, -1), std::make_pair(-1, 0), std::make_pair(-1, 1), std::make_pair(0, -1),
      std::make_pair(0, 1),   std::make_pair(1, -1), std::make_pair(1, 0),  std::make_pair(1, 1)};
    bool stabilized = false;
    while (!stabilized) {
        stabilized = true;
        std::vector<std::string> next(grid);
        for (std::size_t i = 0; i < next.size(); ++i) {
            for (std::size_t j = 0; j < next[i].size(); ++j) {
                if (next[i][j] == FLOOR)
                    continue;
                std::size_t num_neighbors =
                  std::count_if(trajectories.begin(), trajectories.end(),
                                [i, j, &grid](const std::pair<long, long>& p) {
#if DAY_11 == 1
                                    return i + p.first < grid.size()
                                        && j + p.second < grid[i].size()
                                        && grid[i + p.first][j + p.second] == FULL;
#elif DAY_11 == 2
                                           std::size_t x(i), y(j);
                                    x += p.first;
                                    y += p.second;
                                    while (x < grid.size() && y < grid[x].size() && grid[x][y] == FLOOR) {
                                    x += p.first;
                                    y += p.second;
                                    }
                                    return x < grid.size() && y < grid[x].size() && grid[x][y] == FULL;
#endif
                                });
                bool fill = (grid[i][j] == EMPTY && num_neighbors == 0)
                         || (grid[i][j] == FULL && num_neighbors < THRESHOLD);
                if (fill != (grid[i][j] == FULL))
                    stabilized = false;
                next[i][j] = fill ? FULL : EMPTY;
            }
        }
        grid = std::move(next);
    }

    std::cout << std::transform_reduce(
      grid.begin(), grid.end(), 0UL, std::plus<std::size_t>(), [&FULL](const std::string& s) {
          return std::count(s.begin(), s.end(), FULL);
      }) << std::endl;

    return 0;
}

#endif // DAY_11
