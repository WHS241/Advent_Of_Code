#ifndef DAY_23
#define DAY_23

#include <iostream>
#include <string>

#include "assembunny.cpp"

int day_23_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    assembunny_computer runner(argv[1]);
    std::size_t initial_value = std::stoul(argv[2]);

    runner.registers[0] = initial_value;

    runner.run();

    std::cout << runner.registers[0] << std::endl;

    return 0;
}

#endif // DAY_23
