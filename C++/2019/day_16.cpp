#ifndef DAY_16
#define DAY_16 2

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int day_16_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    constexpr std::size_t NUM_PHASES = 100;
#if DAY_16 == 1
    constexpr std::size_t NUM_DUPS = 1;
#elif DAY_16 == 2
    constexpr std::size_t NUM_DUPS = 10000;
#endif
    constexpr std::size_t NUM_OFFSET_DIGITS = 7;
    constexpr std::size_t MESSAGE_SIZE = 8;

    std::vector<uint8_t> input;
    std::ifstream reader(argv[1]);
    for (std::string line; std::getline(reader, line);) {
        for (char c : line) {
            if (!std::isdigit(c))
                throw std::invalid_argument("不是数字");
            input.push_back(c - '0');
        }
    }
    // std::cout << input.size() * NUM_DUPS << std::endl;

    std::size_t offset = 0;

#if DAY_16 == 2
    for (std::size_t i = 0; i < NUM_OFFSET_DIGITS; ++i)
        offset = 10 * offset + input[i];
#endif

    std::vector<uint8_t> transmission;
    for (std::size_t i = offset; i < NUM_DUPS * input.size(); ++i)
        transmission.push_back(input[i % input.size()]);

    // std::cout << transmission.size() << std::endl;
    // std::copy(transmission.begin(), transmission.end(), std::ostream_iterator<int>(std::cout));
    // std::cout << std::endl;

    for (std::size_t i = 0; i < NUM_PHASES; ++i) {
        std::vector<int> next(transmission.size());
        next.back() = transmission.back();
        for (std::size_t j = next.size(); j > 0; --j) {
            next[j - 1] = next[j];
            std::size_t curr = j - 1;
            std::size_t effective_index = curr + offset;

            // Adjustments
            for (std::size_t k = 0; k * (effective_index + 1) < transmission.size() + offset; ++k) {
                for (std::size_t adjuster = (k + 1) * (effective_index + 1) - 1;
                     adjuster < (k + 1) * (effective_index + 2) - 1
                     && adjuster < transmission.size() + offset;
                     ++adjuster) {
                    uint8_t value = transmission[adjuster - offset];

                    if (((k + 1) / 2) % 2 == 0) {
                        next[curr] += value;
                    } else {
                        next[curr] -= value;
                    }
                }
            }
        }

        std::transform(next.begin(), next.end(), transmission.begin(),
                       [](int i) -> uint8_t { return std::abs(i) % 10; });

        // std::copy(transmission.begin(), transmission.end(),
        // std::ostream_iterator<int>(std::cout)); std::cout << std::endl;
    }

    std::copy_n(transmission.begin(), MESSAGE_SIZE, std::ostream_iterator<int>(std::cout));
    std::cout << std::endl;

    return 0;
}

#endif // DAY_16
