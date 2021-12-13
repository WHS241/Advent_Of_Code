#ifndef DAY_2
#define DAY_2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t total_part_1(0), total_part_2(0);
    for (std::string line; std::getline(reader, line);) {
        std::array<std::size_t, 3> dims;
        std::istringstream buf(line);

        std::generate(dims.begin(), dims.end(), [&buf]() {
            std::string temp;
            std::getline(buf, temp, 'x');
            return std::stoul(temp);
        });

        std::array<std::size_t, 3> areas;
        std::size_t pos = 0;
        for (std::size_t i = 0; i < dims.size(); ++i)
            for (std::size_t j = i + 1; j < dims.size(); ++j)
                areas[pos++] = 2 * dims[i] * dims[j];

        total_part_1 += std::accumulate(areas.begin(), areas.end(), 0)     // surface area
                      + *std::min_element(areas.begin(), areas.end()) / 2; // slack

        total_part_2 +=
          2 * std::accumulate(dims.begin(), dims.end(), 0)
          - 2 * *std::max_element(dims.begin(), dims.end()) // shortest face perim
          + std::accumulate(dims.begin(), dims.end(), 1, std::multiplies<std::size_t>()); // bow
    }

    std::cout << "Part 1: " << total_part_1 << "\nPart 2: " << total_part_2 << std::endl;

    return 0;
}

#endif // DAY_2
