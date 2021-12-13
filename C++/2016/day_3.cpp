#ifndef DAY_3
#define DAY_3

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>

int day_3_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t num_valid_triangles_p1(0), num_valid_triangles_p2(0);

    std::vector<std::vector<std::size_t>> values(3);

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);

        std::vector<std::size_t> sides((std::istream_iterator<std::size_t>(line_read)),
                                       std::istream_iterator<std::size_t>());

        // P1: Using triangle inequality
        if (std::accumulate(sides.begin(), sides.end(), 0UL)
            > 2 * *std::max_element(sides.begin(), sides.end()))
            ++num_valid_triangles_p1;

        // P2: Transpose before tri-ineq
        for (std::size_t i = 0; i < sides.size(); ++i)
            values[i].push_back(sides[i]);
        if (values[0].size() == 3) {
            for (std::vector<std::size_t>& t : values) {
                if (std::accumulate(t.begin(), t.end(), 0UL)
                    > 2 * *std::max_element(t.begin(), t.end()))
                    ++num_valid_triangles_p2;
                t.clear();
            }
        }
    }

    std::cout << num_valid_triangles_p1 << '\n' << num_valid_triangles_p2 << std::endl;

    return 0;
}

#endif
