#ifndef DAY_17
#define DAY_17

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

int day_17_main(int argc, char** argv) {
    if (argc < 5)
        return 1;

    long min_x = std::stol(argv[1]);
    long max_x = std::stol(argv[2]);
    long min_y = std::stol(argv[3]);
    long max_y = std::stol(argv[4]);

    long best_y = 0;
    long best_found = 0;
    long best_found_x;

    std::size_t num_found = 0;

    for (long curr_x = std::sqrt(min_x); curr_x <= max_x; ++curr_x) {
        // will x velocity skip entire section?
        bool in_target = false;
        long v_x = curr_x, pos_x = 0;
        while (!in_target && v_x != 0 && pos_x <= max_x) {
            pos_x += v_x;
            --v_x;
            in_target = (min_x <= pos_x && pos_x <= max_x);
        }
        if (!in_target)
            continue;

        bool overshot = false;
        long curr_y = min_y;
        while (!overshot) {
            bool reached_zone = true;
            long vel_x = curr_x, vel_y = curr_y;
            long pos_x = 0, pos_y = 0;
            long max_alt = 0;
            while (reached_zone) {
                pos_x += vel_x;
                if (vel_x > 0)
                    --vel_x;
                pos_y += vel_y;
                --vel_y;
                max_alt = std::max(pos_y, max_alt);

                if (min_x <= pos_x && pos_x <= max_x && min_y <= pos_y && pos_y <= max_y)
                    break;
                if ((pos_x > max_x) || (pos_y < min_y && vel_y <= 0))
                    reached_zone = false;
            }

            if (reached_zone) {
                ++num_found;
                if (max_alt > best_y) {
                    best_y = max_alt;
                    best_found = curr_y;
                    best_found_x = curr_x;
                }
            }
            if ((pos_x > max_x && pos_y > max_y) || (pos_y < min_y && vel_y <= pos_y))
                overshot = true;
            ++curr_y;
        }
    }

    std::cout << best_y << ":" << best_found_x << ',' << best_found << '\n'
              << num_found << std::endl;
    return 0;
}

#endif // DAY_17
