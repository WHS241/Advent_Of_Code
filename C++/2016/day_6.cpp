#ifndef DAY_6
#define DAY_6

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>

int day_6_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::unordered_map<char, std::size_t>> counters;

    for (std::string line; std::getline(reader, line);) {
        if (counters.empty())
            counters.resize(line.size());
        for (std::size_t i = 0; i < line.size(); ++i)
            ++counters[i][line[i]];
    }

    std::string msg_p1, msg_p2;

    for (const auto& m : counters) {
        auto vals = std::minmax_element(m.begin(), m.end(),
                                        [](auto x, auto y) { return x.second < y.second; });
        msg_p1.push_back(vals.second->first);
        msg_p2.push_back(vals.first->first);
    }

    std::cout << msg_p1 << '\n' << msg_p2 << std::endl;

    return 0;
}

#endif
