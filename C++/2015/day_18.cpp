#ifndef DAY_18
#define DAY_18 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int day_18_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    constexpr char ALIVE = '#';

    std::ifstream reader(argv[1]);
    std::size_t num_generations = std::stoul(argv[2]);

    std::vector<std::vector<bool>> alive;

    for (std::string line; std::getline(reader, line);) {
        alive.emplace_back(line.size());
        std::transform(line.begin(), line.end(), alive.back().begin(),
                       [](char c) { return c == ALIVE; });
    }

#if DAY_18 == 2
    alive.front().front() = true;
    alive.front().back() = true;
    alive.back().front() = true;
    alive.back().back() = true;
#endif

    for (std::size_t i = 0; i < num_generations; ++i) {
        std::vector<std::vector<bool>> next(alive.size(), std::vector<bool>(alive[0].size()));
        for (std::size_t j = 0; j < next.size(); ++j) {
            for (std::size_t k = 0; k < next[j].size(); ++k) {
                std::size_t alive_neighbors = 0;
                bool first_row = (j == 0);
                bool first_col = (k == 0);
                bool last_row = (j == alive.size() - 1);
                bool last_col = (k == alive[j].size() - 1);
#if DAY_18 == 2
                if ((first_row || last_row) && (first_col || last_col)) {
                    next[j][k] = true;
                    continue;
                }
#endif

                if (!first_row) {
                    if (alive[j - 1][k])
                        ++alive_neighbors;
                    if (!first_col && alive[j - 1][k - 1])
                        ++alive_neighbors;
                    if (!last_col && alive[j - 1][k + 1])
                        ++alive_neighbors;
                }
                if (!last_row) {
                    if (alive[j + 1][k])
                        ++alive_neighbors;
                    if (!first_col && alive[j + 1][k - 1])
                        ++alive_neighbors;
                    if (!last_col && alive[j + 1][k + 1])
                        ++alive_neighbors;
                }
                if (!first_col && alive[j][k - 1])
                    ++alive_neighbors;
                if (!last_col && alive[j][k + 1])
                    ++alive_neighbors;
                next[j][k] = (alive_neighbors == 3) || (alive[j][k] && alive_neighbors == 2);
            }
        }

        alive = next;
    }

    for (const std::vector<bool>& v : alive) {
        for (bool b : v) {
            std::cout << (b ? ALIVE : '.');
        }
        std::cout << std::endl;
    }

    std::cout << std::accumulate(
      alive.begin(), alive.end(), 0U, [](std::size_t i, const std::vector<bool>& v) {
          return i + std::count(v.begin(), v.end(), true);
      }) << std::endl;
    return 0;
}

#endif // DAY_!8
