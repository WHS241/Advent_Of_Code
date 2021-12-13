#ifndef DAY_6
#define DAY_6

#include <fstream>
#include <iostream>
#include <unordered_set>

int day_6_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_set<char> current_1, current_2;

    std::size_t total_1 = 0, total_2 = 0;
    bool is_first = true;

    for (std::string line; std::getline(reader, line);) {
        if (line.empty()) {
            total_1 += current_1.size();
            current_1.clear();
            total_2 += current_2.size();
            current_2.clear();
            is_first = true;
        } else if (is_first) {
            for (char c : line) {
                current_1.insert(c);
                current_2.insert(c);
            }
            is_first = false;
        } else {
            std::unordered_set<char> temp;
            for (char c : line) {
                current_1.insert(c);
                if (current_2.count(c))
                    temp.insert(c);
            }
            std::swap(current_2, temp);
        }
    }

    total_1 += current_1.size();
    total_2 += current_2.size();

    std::cout << total_1 << '\n' << total_2 << std::endl;

    return 0;
}

#endif // DAY_6
