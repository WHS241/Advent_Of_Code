#ifndef DAY_6
#define DAY_6

#define DAY_6_PART 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>

constexpr std::size_t GRID_X(1000), GRID_Y(1000);

int day_6_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

// too much memory: need preprocessor directives to avoid segfault
#if DAY_6_PART == 1
    std::array<std::array<bool, GRID_Y>, GRID_X> lights_1;
    for (std::array<bool, GRID_Y>& r : lights_1)
        std::fill(r.begin(), r.end(), false);
#elif DAY_6_PART == 2
    std::array<std::array<std::size_t, GRID_Y>, GRID_X> lights_2;
    for (std::array<std::size_t, GRID_Y>& r : lights_2)
        std::fill(r.begin(), r.end(), 0);
#endif

    std::ifstream reader(argv[1]);
    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_parse(line);
        std::string word;
        line_parse >> word;
        if (word == "turn") {
            line_parse >> word;
            bool setting = (word == "on");

            // target area boundaries
            std::size_t ur_x, ur_y, dl_x, dl_y;
            line_parse >> word;
            std::size_t delim = word.find(',');
            dl_x = std::stoul(word.substr(0, delim));
            dl_y = std::stoul(word.substr(delim + 1));
            line_parse >> word >> word;
            delim = word.find(',');
            ur_x = std::stoul(word.substr(0, delim));
            ur_y = std::stoul(word.substr(delim + 1));

            for (std::size_t i = dl_x; i <= ur_x; ++i)
                for (std::size_t j = dl_y; j <= ur_y; ++j)
#if DAY_6_PART == 1
                    lights_1[i][j] = setting;
#elif DAY_6_PART == 2
                    if (setting)
                        ++lights_2[i][j];
                    else if (lights_2[i][j] != 0)
                        --lights_2[i][j];
#endif
        } else { // "toggle"
            // target area boundaries
            std::size_t ur_x, ur_y, dl_x, dl_y;
            line_parse >> word;
            std::size_t delim = word.find(',');
            dl_x = std::stoul(word.substr(0, delim));
            dl_y = std::stoul(word.substr(delim + 1));
            line_parse >> word >> word;
            delim = word.find(',');
            ur_x = std::stoul(word.substr(0, delim));
            ur_y = std::stoul(word.substr(delim + 1));

            for (std::size_t i = dl_x; i <= ur_x; ++i)
                for (std::size_t j = dl_y; j <= ur_y; ++j)
#if DAY_6_PART == 1
                    lights_1[i][j] = !lights_1[i][j];
#elif DAY_6_PART == 2
                    lights_2[i][j] += 2;
#endif
        }
    }

    std::cout <<
#if DAY_6_PART == 1
      std::transform_reduce(
        lights_1.begin(), lights_1.end(), std::size_t(0), std::plus<>(),
        [](const std::array<bool, GRID_Y>& b) { return std::count(b.begin(), b.end(), true); })
#elif DAY_6_PART == 2
      std::transform_reduce(lights_2.begin(), lights_2.end(), std::size_t(0), std::plus<>(),
                            [](const std::array<std::size_t, GRID_Y>& b) {
                                return std::accumulate(b.begin(), b.end(), std::size_t(0));
                            })
#endif
              << std::endl;

    return 0;
}

#endif // DAY_6
