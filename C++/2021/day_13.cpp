#ifndef DAY_13
#define DAY_13

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::pair<int, int>> dots;

    for (std::string s; std::getline(reader, s);) {
        if (s.empty())
            break;

        dots.emplace_back();
        std::istringstream parser(s);
        char ignore;
        parser >> dots.back().first >> ignore >> dots.back().second;
    }

    bool is_first = true;
    for (std::string s; std::getline(reader, s);) {
        std::istringstream parser(s);
        std::string ignore;
        std::string instr;
        parser >> ignore >> ignore >> instr;
        int line_num = std::stoi(instr.substr(2));

        for (auto& p : dots) {
            if (instr.front() == 'x' && p.first > line_num) {
                p.first = 2 * line_num - p.first;
            } else if (instr.front() == 'y' && p.second > line_num) {
                p.second = 2 * line_num - p.second;
            }
        }

        if (is_first) {
            std::unordered_map<int, std::unordered_set<int>> folded_dots;
            for (const auto& p : dots)
                folded_dots[p.first].insert(p.second);
            std::cout << std::transform_reduce(folded_dots.begin(), folded_dots.end(), 0UL,
                                               std::plus<>(),
                                               [](const auto& p) { return p.second.size(); })
                      << '\n';
        }
        is_first = false;
    }

    std::map<int, std::set<int>> folded;
    for (const auto& p : dots)
        folded[p.first].insert(p.second);

    int min_x = folded.begin()->first;
    int max_x = (--folded.end())->first;
    int min_y =
      *(std::min_element(folded.begin(), folded.end(), [](const auto& p1, const auto& p2) {
            return *p1.second.begin() < *p2.second.begin();
        })->second.begin());
    int max_y =
      *--(std::max_element(folded.begin(), folded.end(), [](const auto& p1, const auto& p2) {
              return *--p1.second.end() < *--p2.second.end();
          })->second.end());
    std::vector<std::vector<char>> display(max_x - min_x + 1,
                                           std::vector<char>(max_y - min_y + 1, ' '));
    for (const auto& p : folded)
        for (const auto& y : p.second)
            display[p.first - min_x][y - min_y] = '#';

    for (auto& r : display) {
        std::copy(r.begin(), r.end(), std::ostream_iterator<char>(std::cout, ""));
        std::cout << '\n';
    }

    return 0;
}

#endif // DAY_13
