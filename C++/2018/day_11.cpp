#ifndef DAY_11
#define DAY_11 2

#include <iostream>
#include <limits>
#include <vector>

long get_power(std::size_t x, std::size_t y, std::size_t serial) {
    long rack_id = x + 10;
    return (rack_id * long(y) + long(serial)) * rack_id / 100 % 10 - 5;
}

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    constexpr std::size_t GRID_SIZE = 300;

    std::size_t serial = std::stoul(argv[1]);

    std::vector<std::vector<long>> power_grid(GRID_SIZE, std::vector<long>(GRID_SIZE));
    for (std::size_t i = 0; i < GRID_SIZE; ++i)
        for (std::size_t j = 0; j < GRID_SIZE; ++j)
            power_grid[i][j] = get_power(i, j, serial);

    std::size_t out_x(0), out_y(0);

#if DAY_11 == 1
    constexpr std::size_t BOX_SIZE = 3;
    std::vector<std::vector<long>> totals(GRID_SIZE - BOX_SIZE + 1,
                                          std::vector<long>(GRID_SIZE - BOX_SIZE + 1));

    for (std::size_t i = 0; i < totals.size(); ++i)
        for (std::size_t j = 0; j < totals.size(); ++j)
            for (std::size_t k = 0; k < BOX_SIZE; ++k)
                for (std::size_t l = 0; l < BOX_SIZE; ++l)
                    totals[i][j] += power_grid[i + k][j + l];

    for (std::size_t i = 0; i < totals.size(); ++i) {
        for (std::size_t j = 0; j < totals.size(); ++j) {
            if (totals[i][j] > totals[out_x][out_y]) {
                out_x = i;
                out_y = j;
            }
        }
    }
#elif DAY_11 == 2
    std::size_t best_size;
    long value = std::numeric_limits<long>::min();

    std::vector<std::vector<long>> sums(power_grid);
    for (std::size_t s = 1; s <= GRID_SIZE; ++s) {
        if (s != 1) {
            sums.pop_back();
            for (auto& r : sums)
                r.pop_back();
            for (std::size_t i = 0; i < sums.size(); ++i) {
                for (std::size_t j = 0; j < sums.size(); ++j) {
                    for (std::size_t k = 0; k < s; ++k)
                        sums[i][j] += power_grid[i + k][j + s - 1] + power_grid[i + s - 1][j + k];

                    sums[i][j] -= power_grid[i + s - 1][j + s - 1]; // Double count
                }
            }
        }

        for (std::size_t i = 0; i < sums.size(); ++i) {
            for (std::size_t j = 0; j < sums.size(); ++j) {
                if (sums[i][j] > value) {
                    out_x = i;
                    out_y = j;
                    best_size = s;
                    value = sums[i][j];
                }
            }
        }
    }
#endif

    std::cout << out_x << ',' << out_y
#if DAY_11 == 2
              << ',' << best_size
#endif
              << std::endl;

    return 0;
}

#endif // DAY_11
