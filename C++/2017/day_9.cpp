#ifndef DAY_9
#define DAY_9 2

#include <fstream>
#include <iostream>

constexpr char GR_S('{'), GR_T('}'), GA_S('<'), GA_T('>'), IGNO('!');

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    bool in_garbage           = false;
    bool ignore_next          = false;
    std::size_t count_levels  = 0;
    std::size_t level         = 0;
    std::size_t count_garbage = 0;

    for (char c; reader >> c;) {
        if (in_garbage) {
            if (ignore_next)
                ignore_next = false;
            else if (c == IGNO)
                ignore_next = true;
            else if (c == GA_T)
                in_garbage = false;
            else
                ++count_garbage;
        } else {
            if (c == GA_S) {
                in_garbage = true;
            } else if (c == GR_S) {
                ++level;
            } else if (c == GR_T) {
                count_levels += level;
                --level;
            }
        }
    }

#if DAY_9 == 1
    std::cout << count_levels << std::endl;
#elif DAY_9 == 2
    std::cout << count_garbage << std::endl;
#endif

    return 0;
}

#endif // DAY_9
