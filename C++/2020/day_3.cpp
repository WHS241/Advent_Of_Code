#ifndef DAY_3
#define DAY_3

#include <fstream>
#include <iostream>
#include <vector>

int day_3_main(int argc, char** argv) {
    if (argc < 4)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> trees;
    constexpr char TREE = '#';

    for (std::string line; std::getline(reader, line);)
        trees.push_back(std::move(line));

    // P1: dx = 3, dy = 1; P2: varied
    std::size_t dx(std::stoul(argv[2])), dy(std::stoul(argv[3]));

    std::size_t x(0), y(0), count(0);
    while (y < trees.size()) {
        if (trees[y][x] == TREE)
            ++count;
        x = (x + dx) % trees[y].size(); // wrap around
        y += dy;
    }

    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_3
