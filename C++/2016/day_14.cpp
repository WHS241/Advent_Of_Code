#ifndef DAY_14
#define DAY_14

#include <array>
#include <cctype>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>

#include "util.h"

std::size_t to_hex(char c) {
    return std::isdigit(c) ? c - '0' : (10 + (std::isupper(c) ? c - 'A' : c - 'a'));
}

int day_14_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::string prefix = argv[1];
    std::size_t num_passwords = std::stoul(argv[2]);
    std::size_t num_stretches = argc == 3 ? 0 : std::stoul(argv[3]);

    constexpr std::size_t MAX_DIST = 1000;
    constexpr std::size_t LOWER_TRIGGER = 3;
    constexpr std::size_t UPPER_TRIGGER = 5;

    std::set<std::size_t> valid;
    std::array<std::list<std::size_t>, 16> potential_keys;

    std::size_t upper_bound = -1UL;
    for (std::size_t i = 0; i < upper_bound; ++i) {
        std::string hash_value = hex_string_hash(prefix + std::to_string(i), num_stretches);

        bool found_low = false;
        char curr = hash_value[0];
        std::size_t count = 0;

        for (char c : hash_value) {
            if (c == curr) {
                std::size_t decimal = to_hex(c);
                ++count;
                if (!found_low && count == LOWER_TRIGGER) {
                    found_low = true;
                    potential_keys[decimal].push_back(i);
                }
                if (count == UPPER_TRIGGER) {
                    while (i > potential_keys[decimal].front() + MAX_DIST)
                        potential_keys[decimal].pop_front();
                    for (std::size_t key : potential_keys[decimal])
                        if (key != i) {
                            valid.insert(key);
                        }
                }
                while (valid.size() > num_passwords)
                    valid.erase(--valid.end());
                if (valid.size() == num_passwords)
                    upper_bound = MAX_DIST + *(--valid.end());
            } else {
                curr = c;
                count = 1;
            }
        }
    }

    std::cout << *(--valid.end()) << std::endl;

    return 0;
}
#endif // DAY_14
