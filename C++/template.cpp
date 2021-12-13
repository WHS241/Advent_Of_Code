#ifndef DAY_x
#define DAY_x

#include <fstream>
#include <iostream>

int day_x_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    return 0;
}

#endif // DAY_x
