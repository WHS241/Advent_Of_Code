#ifndef DAY_8
#define DAY_8

#include <fstream>
#include <iostream>
#include <vector>

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::ifstream reader(argv[1]);

    std::size_t count_extra_1 = 0, count_extra_2 = 0;

    bool escaped = false;
    for (std::string line; std::getline(reader, line);) {
        // account for wrapping quotes
        count_extra_1 += 2;
        count_extra_2 += 2;
        for (char c : line) {
            if (c == '\\') {
                if (!escaped)
                    ++count_extra_1;
                escaped = !escaped;
                ++count_extra_2;
            } else {
                if (escaped && c == 'x')
                    count_extra_1 += 2;
                escaped = false;

                if (c == '"')
                    ++count_extra_2;
            }
        }
    }

    std::cout << count_extra_1 << '\n' << count_extra_2 << std::endl;

    return 0;
}

#endif // DAY_8
