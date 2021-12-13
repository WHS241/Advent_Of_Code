#ifndef DAY_2
#define DAY_2 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <vector>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t total = 0;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);

#if DAY_2 == 1
        std::vector<int> values(std::istream_iterator<int>{line_read},
                                std::istream_iterator<int>{});
        total += *std::max_element(values.begin(), values.end()) -
                 *std::min_element(values.begin(), values.end());
#elif DAY_2 == 2
        std::set<int> values(std::istream_iterator<int>{line_read}, std::istream_iterator<int>{});
        for (auto it = values.begin(); it != values.end(); ++it)
            for (auto it2 = values.begin(); it2 != it; ++it2)
                if (*it % *it2 == 0)
                    total += *it / *it2;
#endif
    }

    std::cout << total << std::endl;

    return 0;
}

#endif // DAY_2
