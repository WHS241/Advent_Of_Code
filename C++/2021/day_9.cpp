#ifndef DAY_9
#define DAY_9

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <vector>

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> dangers(std::istream_iterator<std::string>{reader},
                                     std::istream_iterator<std::string>{});

    std::size_t part_1 = 0;

    for (std::size_t i = 0; i < dangers.size(); ++i) {
        for (std::size_t j = 0; j < dangers[i].size(); ++j) {
            std::vector<char> neighbors;
            neighbors.reserve(8);
            if (i > 0) {
                neighbors.push_back(dangers[i - 1][j]);
            }
            if (i < dangers.size() - 1) {
                neighbors.push_back(dangers[i + 1][j]);
            }
            if (j > 0)
                neighbors.push_back(dangers[i][j - 1]);
            if (j < dangers[i].size() - 1)
                neighbors.push_back(dangers[i][j + 1]);

            if (dangers[i][j] < *std::min_element(neighbors.begin(), neighbors.end()))
                part_1 += 1 + dangers[i][j] - '0';
        }
    }

    std::cout << part_1 << '\n';

    std::list<std::pair<std::size_t, std::size_t>> to_fill;
    std::vector<std::size_t> basin_sizes;

    for (std::size_t i = 0; i < dangers.size(); ++i) {
        for (std::size_t j = 0; j < dangers[i].size(); ++j) {
            basin_sizes.push_back(0);
            to_fill.emplace_back(i, j);
            while (!to_fill.empty()) {
                std::pair<std::size_t, std::size_t> curr = to_fill.front();
                to_fill.pop_front();
                if (dangers[curr.first][curr.second] != '9'
                    && dangers[curr.first][curr.second] != ' ') {
                    ++basin_sizes.back();
                    dangers[curr.first][curr.second] = ' ';
                    if (curr.first > 0)
                        to_fill.emplace_back(curr.first - 1, curr.second);
                    if (curr.first < dangers.size() - 1)
                        to_fill.emplace_back(curr.first + 1, curr.second);
                    if (curr.second > 0)
                        to_fill.emplace_back(curr.first, curr.second - 1);
                    if (curr.second < dangers[curr.first].size() - 1)
                        to_fill.emplace_back(curr.first, curr.second + 1);
                }
            }
        }
    }

    std::sort(basin_sizes.begin(), basin_sizes.end());
    std::size_t part_2 =
      std::accumulate(basin_sizes.rbegin(), basin_sizes.rbegin() + 3, 1UL, std::multiplies<>());
    std::cout << part_2 << std::endl;

    return 0;
}

#endif // DAY_9
