#ifndef DAY_21
#define DAY_21

#include <climits>
#include <iostream>

#include "intcode.cpp"

int day_21_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::string input_str;
    auto it = input_str.end();

    auto input = [&input_str, &it]() {
        while (it == input_str.end()) {
            std::cout << "输入：";
            if (!std::getline(std::cin, input_str))
                std::abort();
            if (!input_str.empty())
                input_str.push_back('\n');
            it = input_str.begin();
        }

        return *(it++);
    };

    auto output = [](long x) {
        if (x < CHAR_MAX) {
            std::cout << (char) x;
        } else {
            std::cout << x;
        }
    };

    intcode_computer runner(argv[1], input, output);
    runner.run();
    std::cout << std::endl;

    return 0;
}

#endif // DAY_21
