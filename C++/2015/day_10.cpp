#ifndef DAY_10
#define DAY_10

#include <iostream>
#include <string>

int day_10_main(int argc, char** argv) {
    if (argc < 3)
        return 1;
    std::string curr = argv[1];
    std::size_t num_it = std::stoul(argv[2]); // P1: 10; P2: 50

    for (std::size_t i = 0; i < num_it; ++i) {
        char curr_digit = curr[0];
        std::size_t count = 1;

        std::string next;

        for (std::size_t j = 1; j < curr.size(); ++j) {
            if (curr[j] == curr_digit) {
                ++count;
            } else {
                next += std::to_string(count);
                next.push_back(curr_digit);
                curr_digit = curr[j];
                count = 1;
            }
        }
        next += std::to_string(count);
        next.push_back(curr_digit);

        curr = next;
    }

    std::cout << curr.length() << std::endl;

    return 0;
}

#endif // DAY_10
