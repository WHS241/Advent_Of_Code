#ifndef DAY_1
#define DAY_1

#include <fstream>
#include <iostream>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    int curr_level = 0;
    std::ifstream reader(argv[1]);
    std::size_t first_basement = 0;
    bool reached_basement = false;

    for (std::string line; std::getline(reader, line);) {
        for (char c : line) {
            if (c == ')')
                --curr_level;
            else if (c == '(')
                ++curr_level;

            // Part 2
            if (!reached_basement)
                ++first_basement;
            if (curr_level < 0)
                reached_basement = true;
        }
    }

    std::cout << curr_level << '\n' << first_basement << std::endl;

    return 0;
}

#endif // DAY_1
