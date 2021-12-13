#ifndef DAY_17
#define DAY_17 2

#include "intcode.cpp"
#include <climits>
#include <iostream>

int day_17_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

#if DAY_17 == 1
    auto input = []() {
        return 0;
    };
#elif DAY_17 == 2
    std::vector<std::string> inputs = {{"A,B,B,C,C,A,A,B,B,C\n"},
                                       {"L,12,R,4,R,4\n"},
                                       {"R,12,R,4,L,12\n"},
                                       {"R,12,R,4,L,6,L,8,L,8\n"},
                                       {"n\n"}};

    auto it = inputs.begin();
    auto it2 = it->begin();

    auto input = [&it, &it2]() {
        if (it2 == it->end())
            it2 = (++it)->begin();
        return *(it2++);
    };
#endif

    bool printed = false;
    auto output = [&printed](long i) {
        if (i < CHAR_MAX) {
            if (!std::isspace(i))
                printed = true;

            if (i == '\n' && !printed) {
                std::cout << "\033[2J\033[1;1H";
            } else {
                std::cout << (char) i;
                if (i == '\n')
                    printed = false;
            }
        } else {
            std::cout << i << std::endl;
        }
    };

    intcode_computer runner(argv[1], input, output);
    runner.run();

    return 0;
}

#endif // DAY_17
