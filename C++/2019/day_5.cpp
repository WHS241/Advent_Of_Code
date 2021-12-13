#ifndef DAY_5
#define DAY_5

#include "intcode.cpp"

int day_5_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    int input_ID = std::stoi(argv[2]);

    auto get_input = [input_ID]() {
        return input_ID;
    };
    auto output = [](int x) {
        std::cout << x << std::endl;
    };

    intcode_computer runner(argv[1], get_input, output);
    runner.run();

    return 0;
}

#endif // DAY_5
