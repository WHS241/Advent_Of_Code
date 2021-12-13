#ifndef DAY_19
#define DAY_19

#include <array>
#include <iostream>

#include "intcode.cpp"

bool is_affected(std::size_t i, std::size_t j, char* file_path) {
    bool reported_x = false;
    bool ret_val;
    auto input = [&i, &j, &reported_x]() {
        reported_x = !reported_x;

        return reported_x ? i : j;
    };

    auto output = [&ret_val](long v) {
        ret_val = v;
    };

    intcode_computer runner(file_path, input, output);
    runner.run();

    return ret_val;
}

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    constexpr std::size_t X_MIN = 8;
    constexpr std::size_t Y_MIN = 9;
    constexpr std::size_t TARGET_SIZE = 100;

    std::vector<std::pair<std::size_t, std::size_t>> bounds;

    std::size_t curr_min_x = X_MIN;
    std::size_t curr_max_x = X_MIN + 1;
    std::size_t curr_y = Y_MIN;

    while (true) {
        while (!is_affected(curr_min_x, curr_y, argv[1]))
            ++curr_min_x;
        while (is_affected(curr_max_x, curr_y, argv[1]))
            ++curr_max_x;

        bounds.emplace_back(curr_min_x, curr_max_x);

        if (bounds.back().second - bounds.back().first >= TARGET_SIZE
            && curr_y - Y_MIN + 1 >= TARGET_SIZE) {
            // if bounds are valid, the square fits
            std::pair<std::size_t, std::size_t>& lower_edge =
              bounds[curr_y - Y_MIN - TARGET_SIZE + 1];

            if (lower_edge.first <= bounds.back().first && lower_edge.second <= bounds.back().second
                && lower_edge.second - bounds.back().first >= TARGET_SIZE) {
                std::cout << "X: " << bounds.back().first << ", Y: " << curr_y - TARGET_SIZE + 1
                          << std::endl;
                break;
            }
        }

        ++curr_y;
    }
    return 0;
}

#endif // DAY_19
