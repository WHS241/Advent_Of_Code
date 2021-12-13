#ifndef DAY_2
#define DAY_2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t num_valid_1 = 0, num_valid_2 = 0;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string word;
        std::size_t lower, upper;

        line_read >> word;
        std::size_t pos = word.find('-');

        lower = std::stoul(word.substr(0, pos));
        upper = std::stoul(word.substr(pos + 1));

        line_read >> word;
        char target = word.front();

        std::string password;
        line_read >> password;

        std::size_t count = std::count(password.begin(), password.end(), target);
        if (count >= lower && count <= upper)
            ++num_valid_1;
        // Using 1-based indexing
        if ((password.at(lower - 1) == target) ^ (password.at(upper - 1) == target))
            ++num_valid_2;
    }

    std::cout << num_valid_1 << '\n' << num_valid_2 << std::endl;

    return 0;
}

#endif // DAY_2
