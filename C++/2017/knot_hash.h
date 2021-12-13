#ifndef KNOT_HASH
#define KNOT_HASH

#include <algorithm>
#include <array>
#include <numeric>
#include <string>
#include <vector>

constexpr std::size_t KNOT_SIZE = 256;
constexpr std::size_t DENSE_HASH_SIZE = 16;

std::array<uint8_t, DENSE_HASH_SIZE> knot_hash(const std::string& s) {
    std::vector<std::size_t> lengths(s.begin(), s.end());

    std::array<uint8_t, KNOT_SIZE> values;
    std::iota(values.begin(), values.end(), 0);

    std::size_t pos = 0;
    std::size_t skip = 0;

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
                std::vector<uint8_t> buffer(x);
                std::size_t num_back = KNOT_SIZE - pos;
                std::copy(values.begin() + pos, values.end(), buffer.begin());
                std::copy(values.begin(), values.begin() + (x - num_back), buffer.begin() + num_back);
                std::reverse(buffer.begin(), buffer.end());
                std::copy(buffer.begin(), buffer.begin() + num_back, values.begin() + pos);
                std::copy(buffer.begin() + num_back, buffer.end(), values.begin());
            }

            pos = (pos + x + skip++) % KNOT_SIZE;
        }
    }

    constexpr std::size_t BLOCK_SIZE = KNOT_SIZE / DENSE_HASH_SIZE;
    std::array<uint8_t, DENSE_HASH_SIZE> output;
    for (std::size_t i = 0; i < DENSE_HASH_SIZE; ++i)
        output[i] = std::reduce(values.begin() + BLOCK_SIZE * i, values.begin() + BLOCK_SIZE * (i + 1), 0, std::bit_xor<uint8_t>());

    return output;
}

#endif 
