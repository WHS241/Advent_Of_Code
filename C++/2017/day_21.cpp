#ifndef DAY_21
#define DAY_21

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

bool rot_eq(const std::vector<std::string>& x, const std::vector<std::string>& y) {
    if (x.size() != y.size())
        return false;
    if (x == y || std::equal(x.begin(), x.end(), y.rbegin()) ||
        std::equal(x.begin(), x.end(), y.begin(),
                   [](const std::string& x, const std::string& y) {
                       return std::equal(x.begin(), x.end(), y.rbegin(), y.rend());
                   }) ||
        std::equal(x.begin(), x.end(), y.rbegin(), [](const std::string& x, const std::string& y) {
            return std::equal(x.begin(), x.end(), y.rbegin(), y.rend());
        }))
        return true;

    std::vector<std::string> y_t(y);
    try {
        for (std::size_t i = 0; i < y.size(); ++i)
            for (std::size_t j = i + 1; j < y.size(); ++j)
                std::swap(y_t[i].at(j), y_t[j].at(i));
    } catch (std::out_of_range&) { return false; }

    return x == y_t || std::equal(x.begin(), x.end(), y_t.rbegin()) ||
           std::equal(x.begin(), x.end(), y_t.begin(),
                      [](const std::string& x, const std::string& y) {
                          return std::equal(x.begin(), x.end(), y.rbegin());
                      }) ||
           std::equal(x.begin(), x.end(), y_t.rbegin(),
                      [](const std::string& x, const std::string& y) {
                          return std::equal(x.begin(), x.end(), y.rbegin());
                      });
}

int day_21_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_it = std::stoul(argv[2]);

    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> subs2, subs3;

    for (std::string line; std::getline(reader, line);) {
        if (line[2] == '/') {
            subs2.emplace_back(
              std::vector<std::string>{line.substr(0, 2), line.substr(3, 2)},
              std::vector<std::string>{line.substr(9, 3), line.substr(13, 3), line.substr(17, 3)});
        } else {
            subs3.emplace_back(
              std::vector<std::string>{line.substr(0, 3), line.substr(4, 3), line.substr(8, 3)},
              std::vector<std::string>{line.substr(15, 4), line.substr(20, 4), line.substr(25, 4),
                                       line.substr(30, 4)});
        }
    }

    std::vector<std::string> grid = {".#.", "..#", "###"};

    for (std::size_t i = 0; i < num_it; ++i) {
        std::vector<std::vector<std::vector<std::string>>> partitioned;
        if (grid.size() % 2 == 0) {
            partitioned.resize(grid.size() / 2);
            std::fill(partitioned.begin(), partitioned.end(),
                      std::vector<std::vector<std::string>>{grid.size() / 2,
                                                            std::vector<std::string>{2, "  "}});

            for (std::size_t i = 0; i < grid.size(); ++i)
                for (std::size_t j = 0; j < grid.size(); ++j)
                    partitioned[i / 2][j / 2][i % 2][j % 2] = grid[i][j];

            for (auto& a : partitioned) {
                for (auto& s : a) {
                    auto it = std::find_if(
                      subs2.begin(), subs2.end(),
                      [&s](const std::pair<std::vector<std::string>, std::vector<std::string>>& p) {
                          return rot_eq(p.first, s);
                      });
                    s = it->second;
                }
            }

            grid.resize(grid.size() / 2 * 3);
            for (std::size_t i = 0; i < grid.size(); ++i) {
                grid[i].clear();
                for (std::size_t j = 0; j < grid.size(); ++j)
                    grid[i].push_back(partitioned[i / 3][j / 3][i % 3][j % 3]);
            }
        } else {
            partitioned.resize(grid.size() / 3);
            std::fill(partitioned.begin(), partitioned.end(),
                      std::vector<std::vector<std::string>>{grid.size() / 3,
                                                            std::vector<std::string>{3, "   "}});

            for (std::size_t i = 0; i < grid.size(); ++i)
                for (std::size_t j = 0; j < grid.size(); ++j)
                    partitioned[i / 3][j / 3][i % 3][j % 3] = grid[i][j];

            for (auto& a : partitioned) {
                for (auto& s : a) {
                    auto it = std::find_if(
                      subs3.begin(), subs3.end(),
                      [&s](const std::pair<std::vector<std::string>, std::vector<std::string>>& p) {
                          return rot_eq(p.first, s);
                      });
                    s = it->second;
                }
            }

            grid.resize(grid.size() / 3 * 4);
            for (std::size_t i = 0; i < grid.size(); ++i) {
                grid[i].clear();
                for (std::size_t j = 0; j < grid.size(); ++j)
                    grid[i].push_back(partitioned[i / 4][j / 4][i % 4][j % 4]);
            }
        }
    }

    std::copy(grid.begin(), grid.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << std::transform_reduce(
                   grid.begin(), grid.end(), 0UL, std::plus<std::size_t>(),
                   [](const std::string& s) { return std::count(s.begin(), s.end(), '#'); })
              << std::endl;

    return 0;
}

#endif // DAY_21
