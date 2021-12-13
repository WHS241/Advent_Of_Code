#ifndef DAY_19
#define DAY_19

#include <cctype>
#include <fstream>
#include <iostream>
#include <vector>

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::ifstream reader(argv[1]);

    std::vector<std::string> network;

    for (std::string line; std::getline(reader, line);)
        network.push_back(line);

    std::size_t x, y;
    x = 0;
    y = network.front().find_first_not_of(" ");
    bool vertical(true), increasing(true);
    std::string visited;
    std::size_t dist = 0;

    while (x < network.size() && y < network[x].size() && !std::isspace(network[x][y])) {
        if (std::isalpha(network[x][y]))
            visited.push_back(network[x][y]);
        if (network[x][y] == '+') {
            increasing = vertical ? (y < network[x].size() - 1 && !std::isspace(network[x][y + 1]))
                                  : (x < network.size() - 1 && !std::isspace(network[x + 1][y]));
            vertical   = !vertical;
        }
        if (vertical) {
            if (increasing)
                ++x;
            else
                --x;
        } else {
            if (increasing)
                ++y;
            else
                --y;
        }
        ++dist;
    }

    std::cout << visited << std::endl;
    std::cout << dist << std::endl;
    return 0;
}

#endif // DAY_19
