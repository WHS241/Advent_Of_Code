#ifndef DAY_5
#define DAY_5

#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<bool> taken;

    for (std::string line; std::getline(reader, line);) {
        for (char& c : line) {
            if (c == 'R')
                c = 'B';
            else if (c == 'L')
                c = 'F';
        }

        std::bitset<10> val(line.c_str(), std::string::npos, 'F', 'B');
        std::size_t n = val.to_ulong();

        taken.resize(std::max(taken.size(), n + 1), false);
        taken[n] = true;
    }

    std::cout << taken.size() - 1 << '\n';

    for (std::size_t i = 1; i < taken.size() - 1; ++i)
        if (!taken[i] && taken[i - 1] && taken[i + 1])
            std::cout << i << std::endl;

    return 0;
}

#endif // DAY_5
