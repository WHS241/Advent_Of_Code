#ifndef DAY_9
#define DAY_9 2
#include <iostream>

#include "intcode.cpp"

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    auto input = []() {
        return DAY_9;
    };
    auto output = [](long x) {
        std::cout << x << std::endl;
    };
    intcode_computer runner(argv[1], input, output);

    runner.run();

    return 0;
}

#endif // DAY_9
