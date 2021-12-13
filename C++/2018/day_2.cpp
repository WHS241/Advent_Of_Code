#ifndef DAY_2
#define DAY_2 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

#if DAY_2 == 1
    std::size_t c2(0), c3(0);
    for (std::string line; std::getline(reader, line);) {
        std::unordered_map<char, std::size_t> counts;
        for (char c : line)
            ++counts[c];
        if (std::find_if(counts.begin(), counts.end(),
                         [](const std::pair<const char, std::size_t>& p) { return p.second == 2; })
            != counts.end())
            ++c2;
        if (std::find_if(counts.begin(), counts.end(),
                         [](const std::pair<const char, std::size_t>& p) { return p.second == 3; })
            != counts.end())
            ++c3;
    }

    std::cout << c2 * c3 << std::endl;
#elif DAY_2 == 2
    std::unordered_set<std::string> ids;
    for (std::string line; std::getline(reader, line);) {
        for (const std::string& s : ids) {
            if (std::transform_reduce(line.begin(), line.end(), s.begin(), 0UL, std::plus<>(),
                                      [](char c1, char c2) { return c1 != c2; })
                == 1) {
                std::cout << line << '\n' << s << std::endl;
            }
        }
        ids.insert(std::move(line));
    }
#endif

    return 0;
}

#endif // DAY_2
