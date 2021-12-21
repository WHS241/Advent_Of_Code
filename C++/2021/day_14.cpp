#ifndef DAY_14
#define DAY_14

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>

int day_14_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_it = std::stoul(argv[2]);

    std::string input, ignore;

    std::unordered_map<char, std::unordered_map<char, char>> insertion;

    std::getline(reader, input);
    std::getline(reader, ignore);

    for (std::string s; std::getline(reader, s);) {
        insertion[s[0]][s[1]] = s.back();
    }

    std::unordered_map<char, std::unordered_map<char, std::size_t>> pair_count;
    for (std::size_t i = 1; i < input.size(); ++i)
        ++pair_count[input[i - 1]][input[i]];

    for (std::size_t i = 0; i < num_it; ++i) {
        std::unordered_map<char, std::unordered_map<char, std::size_t>> next;
        for (const auto& p1 : insertion) {
            for (const auto& p2 : p1.second) {
                next[p1.first][p2.second] += pair_count[p1.first][p2.first];
                next[p2.second][p2.first] += pair_count[p1.first][p2.first];
            }
        }
        pair_count = std::move(next);
    }

    std::unordered_map<char, std::size_t> counts;
    for (const auto& p1 : pair_count) {
        for (const auto& p2 : p1.second) {
            counts[p2.first] += p2.second;
        }
    }
    ++counts[input.front()];

    auto p = std::minmax_element(counts.begin(), counts.end(),
                                 [](auto p1, auto p2) { return p1.second < p2.second; });
    std::cout << (p.second->second - p.first->second) << std::endl;

    return 0;
}

#endif // DAY_14
