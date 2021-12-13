#ifndef DAY_2
#define DAY_2

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t hor = 0, p1_dep = 0, p2_dep = 0, p2_aim = 0;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream parser(line);

        std::string dir;
        std::size_t dist;

        parser >> dir >> dist;
        if (dir == "forward") {
            hor += dist;
            p2_dep += dist * p2_aim;
        } else if (dir == "up") {
            p1_dep -= dist;
            p2_aim -= dist;
        } else if (dir == "down") {
            p1_dep += dist;
            p2_aim += dist;
        }
    }

    std::cout << hor * p1_dep << '\n' << hor * p2_dep << std::endl;

    return 0;
}

#endif // DAY_2
