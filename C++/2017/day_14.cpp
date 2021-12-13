#ifndef DAY_14
#define DAY_14 2

#include <bitset>
#include <iostream>

#include "knot_hash.h"

constexpr std::size_t ROWS = 128;

int day_14_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::string input(argv[1]);

    std::size_t num_blocks = 0;
    std::array<std::array<bool, ROWS>, ROWS> blocked;
    for (std::size_t i = 0; i < ROWS; ++i) {
        std::array<uint8_t, DENSE_HASH_SIZE> row = knot_hash(input + "-" + std::to_string(i));

        for (std::size_t j = 0; j < DENSE_HASH_SIZE; ++j) {
            uint8_t x = row[j];
            std::bitset<8> b(x);
            num_blocks += b.count();

            for (std::size_t k = 0; k < 8; ++k)
                blocked[i][j * 8 + k] = b[7 - k];
        }
    }

#if DAY_14 == 1
    std::cout << num_blocks << std::endl;
#elif DAY_14 == 2
    std::array<std::array<std::size_t, ROWS>, ROWS> region;
    for (std::array<std::size_t, ROWS>& r : region)
        std::fill(r.begin(), r.end(), 0);

    std::size_t num_regions = 0;
    for (std::size_t i = 0; i < ROWS; ++i) {
        for (std::size_t j = 0; j < ROWS; ++j) {
            if (blocked[i][j] && region[i][j] == 0) {
                ++num_regions;
                std::vector<std::pair<std::size_t, std::size_t>> frontier;
                frontier.emplace_back(i, j);
                region[i][j] = num_regions;
                while (!frontier.empty()) {
                    std::pair<std::size_t, std::size_t> x = frontier.back();
                    frontier.pop_back();

                    std::array<std::pair<std::size_t, std::size_t>, 4> neighbors = {
                      std::make_pair(x.first - 1, x.second), std::make_pair(x.first + 1, x.second),
                      std::make_pair(x.first, x.second - 1), std::make_pair(x.first, x.second + 1)};

                    for (const std::pair<std::size_t, std::size_t>& p : neighbors) {
                        if (p.first < ROWS && p.second < ROWS && blocked[p.first][p.second] &&
                            region[p.first][p.second] == 0) {
                            region[p.first][p.second] = num_regions;
                            frontier.push_back(p);
                        }
                    }
                }
            }
        }
    }

    std::cout << num_regions << std::endl;
#endif

    return 0;
}

#endif // DAY_14
