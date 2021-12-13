#ifndef DAY_13
#define DAY_13

#include <algorithm>
#include <climits>
#include <iostream>
#include <unordered_map>

#include "intcode.cpp"

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::unordered_map<long, std::unordered_map<long, long>> tiles;
    long min_x(LONG_MAX), min_y(LONG_MAX), max_x(LONG_MIN), max_y(LONG_MIN);

    auto input = [&tiles, &min_x, &min_y, &max_x, &max_y]() {
        static const std::string RESET("\033[0m"), EMPTY("\033[40m "), WALL("\033[47m "),
          BLOCK("\033[46m "), PADDLE("\033[42m "), BALL("\033[31m\u2b24");

        std::cout << "\033[2;1H";
        for (long j = min_y; j <= max_y; ++j) {
            for (long i = min_x; i <= max_x; ++i) {
                switch (tiles[i][j]) {
                case 0:
                    std::cout << EMPTY << RESET;
                    break;

                case 1:
                    std::cout << WALL << RESET;
                    break;

                case 2:
                    std::cout << BLOCK << RESET;
                    break;

                case 3:
                    std::cout << PADDLE << RESET;
                    break;

                case 4:
                    std::cout << BALL << RESET;
                    break;

                default:
                    break;
                }
            }
            std::cout << std::endl;
        }

        std::string input;
        std::getline(std::cin, input);
        if (input.empty())
            return 0;

        if (input[0] == 'a')
            return -1;
        if (input[0] == 'd')
            return 1;
        return 0;
    };

    std::size_t counter = 0;
    long x, y;
    auto output = [&counter, &x, &y, &tiles, &min_x, &min_y, &max_x, &max_y](long out) {
        switch (counter) {
        case 0:
            x = out;
            break;

        case 1:
            y = out;
            break;

        case 2:
            if (x == -1 && y == 0) {
                // 重置终端
                std::cout << "\033[2J\033[1;1H";

                std::cout << "分数：" << out << std::endl;
                break;
            }
            tiles[x][y] = out;
            min_x = std::min(min_x, x);
            min_y = std::min(min_y, y);
            max_x = std::max(max_x, x);
            if (out == 3)
                max_y = y;
            break;

        default:
            break;
        }

        counter = (counter + 1) % 3;
    };

    intcode_computer runner(argv[1], input, output);
    runner.run();

    return 0;
}

#endif // DAY_13
