#ifndef DAY_11
#define DAY_11 2

#include <climits>

#include <util/pair_hash.h>
#include "intcode.cpp"

int day_11_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::unordered_map<int, std::unordered_map<int, int>> color_map;
    std::unordered_set<std::pair<int, int>, util::pair_hash<int, int>> colored_squares;

    int curr_x(0), curr_y(0);

    auto input = [&color_map, &curr_x, &curr_y]() {
        return color_map[curr_x][curr_y];
    };

    bool is_color = true;
    std::array<std::pair<int, int>, 4> directions = {std::make_pair(1, 0), std::make_pair(0, 1),
                                                     std::make_pair(-1, 0), std::make_pair(0, -1)};
    std::size_t curr_dir_index = 1;
    auto output = [&is_color, &directions, &curr_dir_index, &curr_x, &curr_y, &color_map,
                   &colored_squares](long x) {
        if (is_color) {
            color_map[curr_x][curr_y] = x;
            colored_squares.emplace(curr_x, curr_y);
        } else {
            curr_dir_index = (curr_dir_index + (x ? 3 : 1)) % 4;
            curr_x += directions[curr_dir_index].first;
            curr_y += directions[curr_dir_index].second;
        }
        is_color = !is_color;
    };

#if DAY_11 == 1
    color_map[0][0] = 0;
#elif DAY_11 == 2
    color_map[0][0] = 1;
#endif

    intcode_computer runner(argv[1], input, output);
    runner.run();

#if DAY_11 == 1
    std::cout << colored_squares.size() << std::endl;
#elif DAY_11 == 2
    int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
    for (const std::pair<int, int>& coord : colored_squares) {
        min_x = std::min(min_x, coord.first);
        min_y = std::min(min_y, coord.second);
        max_x = std::max(max_x, coord.first);
        max_y = std::max(max_y, coord.second);
    }

    for (int i = min_x; i <= max_x; ++i) {
        for (int j = min_y; j <= max_y; ++j)
            std::cout << (color_map[i][j] ? "\033[37m" : "\033[30m") << "\u25A0"
                      << "\033[0m";
        std::cout << std::endl;
    }
#endif

    return 0;
}

#endif // DAY_11
