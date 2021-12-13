#ifndef DAY_11
#define DAY_11

#include <iostream>
#include <list>
#include <string>

bool is_valid(const std::string& s) {
    std::size_t num_pairs = 0;
    std::size_t last_pair = -1UL;
    bool has_increasing_triplet = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == 'i' || s[i] == 'o' || s[i] == 'l')
            return false;
        if (i != 0 && i - 1 != last_pair && s[i - 1] == s[i]) {
            ++num_pairs;
            last_pair = i;
        }
        if (i > 1 && s[i - 2] + 1 == s[i - 1] && s[i - 1] + 1 == s[i])
            has_increasing_triplet = true;
    }

    return has_increasing_triplet && (num_pairs >= 2);
}

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::string input(argv[1]);

    do {
        for (auto it = input.rbegin(); it != input.rend(); ++it) {
            ++(*it);
            if (*it > 'z')
                *it = 'a';
            else
                break;
        }
    } while (!is_valid(input));

    std::cout << input << std::endl;

    return 0;
}

#endif // DAY_11
