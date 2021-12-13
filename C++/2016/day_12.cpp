#ifndef DAY_12
#define DAY_12 2

#include <iostream>

#include "assembunny.cpp"

int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    assembunny_computer runner(argv[1]);

#if DAY_12 == 2
    runner.registers[2] = 1;
#endif

    runner.run();

    std::cout << runner.registers[0] << std::endl;

    return 0;
}

#endif // DAY_12
