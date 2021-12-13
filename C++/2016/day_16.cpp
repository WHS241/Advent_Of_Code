#ifndef DAY_16
#define DAY_16

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int day_16_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::size_t target_length = std::stoul(argv[1]);
    std::string input = argv[2];

    std::vector<bool> bits;
    std::transform(input.begin(), input.end(), std::back_inserter(bits),
                   [](char c) { return c - '0'; });

    while (bits.size() < target_length) {
        std::vector<bool> temp(bits);
        bits.push_back(false);
        std::transform(temp.rbegin(), temp.rend(), std::back_inserter(bits), std::logical_not<>());
    }

    bits.resize(target_length);

    std::vector<bool> checksum(bits);
    while (checksum.size() % 2 == 0) {
        std::vector<bool> temp;
        for (std::size_t i = 0; i < checksum.size(); i += 2)
            temp.push_back(!(checksum[i] ^ checksum[i + 1]));
        checksum = std::move(temp);
    }

    std::copy(checksum.begin(), checksum.end(), std::ostream_iterator<bool>(std::cout, ""));
    std::cout << std::endl;

    return 0;
}

#endif
