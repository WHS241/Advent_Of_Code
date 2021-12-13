#ifndef DAY_8
#define DAY_8

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

constexpr std::size_t SCREEN_HEIGHT = 6;
constexpr std::size_t SCREEN_WIDTH = 50;

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::array<std::array<bool, SCREEN_WIDTH>, SCREEN_HEIGHT> screen;
    for (std::array<bool, SCREEN_WIDTH>& row : screen)
        std::fill(row.begin(), row.end(), false);

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string word;
        line_read >> word;
        if (word == "rect") {
            line_read >> word;
            std::size_t split = word.find('x');
            std::size_t width(std::stoul(word.substr(0, split))),
              height(std::stoul(word.substr(split + 1)));
            for (std::size_t i = 0; i < height; ++i)
                for (std::size_t j = 0; j < width; ++j)
                    screen[i][j] = true;
        } else {
            line_read >> word;
            bool is_row = word == "row";
            line_read.ignore(std::numeric_limits<std::streamsize>::max(), '=');
            line_read >> word;
            std::size_t index = std::stoul(word);
            line_read >> word >> word;
            std::size_t shift = std::stoul(word);

            if (is_row) {
                std::rotate(screen[index].rbegin(), screen[index].rbegin() + shift,
                            screen[index].rend());
            } else {
                std::array<bool, SCREEN_HEIGHT> col;
                std::transform(
                  screen.begin(), screen.end(), col.begin(),
                  [&index](const std::array<bool, SCREEN_WIDTH>& c) { return c[index]; });
                std::rotate(col.rbegin(), col.rbegin() + shift, col.rend());
                for (std::size_t i = 0; i < col.size(); ++i)
                    screen[i][index] = col[i];
            }
        }
    }

    std::size_t count = 0;
    for (const std::array<bool, SCREEN_WIDTH>& row : screen) {
        for (bool b : row) {
            std::cout << (b ? '#' : '.');
            count += b;
        }
        std::cout << '\n';
    }
    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_8
