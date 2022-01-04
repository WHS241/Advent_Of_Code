#ifndef DAY_12
#define DAY_12

#include <iostream>

#include "assembunny.cpp"

int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    assembunny_computer runner(argv[1]);
    runner.run();
    std::cout << runner.registers[0] << std::endl;

    runner = assembunny_computer(argv[1]);
    runner.registers[2] = 1;
    runner.run();
    std::cout << runner.registers[0] << std::endl;

    return 0;
}

#endif // DAY_12
