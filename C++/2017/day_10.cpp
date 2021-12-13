#ifndef DAY_10
#define DAY_10 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    constexpr std::size_t KNOT_SIZE = 256;

    std::array<std::size_t, KNOT_SIZE> values;
    std::iota(values.begin(), values.end(), 0UL);

    std::size_t pos  = 0;
    std::size_t skip = 0;

#if DAY_10 == 1
    for (std::size_t x; reader >> x;) {
        if (pos + x <= KNOT_SIZE) {
            std::reverse(values.begin() + pos, values.begin() + pos + x);
        } else {
            std::vector<std::size_t> buffer(x);
            std::size_t num_back = KNOT_SIZE - pos;
            std::copy(values.begin() + pos, values.end(), buffer.begin());
            std::copy(values.begin(), values.begin() + (x - num_back), buffer.begin() + num_back);
            std::reverse(buffer.begin(), buffer.end());
            std::copy(buffer.begin(), buffer.begin() + num_back, values.begin() + pos);
            std::copy(buffer.begin() + num_back, buffer.end(), values.begin());
        }

        pos = (pos + x + skip++) % KNOT_SIZE;
        reader.ignore(1, ',');
    }

    std::cout << values[0] << "," << values[1] << std::endl;
#elif DAY_10 == 2
    std::vector<std::size_t> lengths(std::istream_iterator<char>{reader},
                                     std::istream_iterator<char>{});
    if (!lengths.empty() && lengths.back() == '\n')
        lengths.pop_back();
    lengths.push_back(17);
    lengths.push_back(31);
    lengths.push_back(73);
    lengths.push_back(47);
    lengths.push_back(23);

    constexpr std::size_t ROUNDS = 64;
    for (std::size_t i = 0; i < ROUNDS; ++i) {
        for (std::size_t x : lengths) {
            if (pos + x <= KNOT_SIZE) {
                std::reverse(values.begin() + pos, values.begin() + pos + x);
            } else {
                std::vector<std::size_t> buffer(x);
                std::size_t num_back = KNOT_SIZE - pos;
                std::copy(values.begin() + pos, values.end(), buffer.begin());
                std::copy(values.begin(), values.begin() + (x - num_back),
                          buffer.begin() + num_back);
                std::reverse(buffer.begin(), buffer.end());
                std::copy(buffer.begin(), buffer.begin() + num_back, values.begin() + pos);
                std::copy(buffer.begin() + num_back, buffer.end(), values.begin());
            }

            pos = (pos + x + skip++) % KNOT_SIZE;
        }
    }

    constexpr std::size_t DENSE_HASH_SIZE = 16;
    constexpr std::size_t BLOCK_SIZE      = KNOT_SIZE / DENSE_HASH_SIZE;
    std::array<std::size_t, DENSE_HASH_SIZE> output;
    for (std::size_t i = 0; i < DENSE_HASH_SIZE; ++i)
        output[i] = std::reduce(values.begin() + BLOCK_SIZE * i,
                                values.begin() + BLOCK_SIZE * (i + 1), 0UL, std::bit_xor<>());

    std::cout << std::hex;
    for (std::size_t x : output) {
        if (x < 0x10)
            std::cout << 0;
        std::cout << x;
    }
    std::cout << std::endl;
#endif

    return 0;
}

#endif // DAY_10
