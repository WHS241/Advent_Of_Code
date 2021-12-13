#ifndef DAY_4
#define DAY_4 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_set>

struct anagram_hash {
    std::size_t operator()(const std::string& s) const {
        return std::transform_reduce(s.begin(), s.end(), 0UL, std::bit_xor<>(), std::hash<char>());
    }
};

struct anagram {
    bool operator()(const std::string& x, const std::string& y) const {
        return std::is_permutation(x.begin(), x.end(), y.begin(), y.end());
    }
};

int day_4_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t count = 0;

    for (std::string line; std::getline(reader, line);) {
#if DAY_4 == 1
        std::unordered_set<std::string> values;
#elif DAY_4 == 2
        std::unordered_set<std::string, anagram_hash, anagram> values;
#endif
        std::istringstream line_read(line);

        bool duplicates = false;
        for (std::string phase; !duplicates && (line_read >> phase);) {
            if (values.count(phase))
                duplicates = true;
            values.insert(phase);
        }

        if (!duplicates)
            ++count;
    }

    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_4
