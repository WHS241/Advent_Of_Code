#ifndef DAY_25
#define DAY_25

#include "assembunny.cpp"
#include <string>

int day_25_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::size_t curr = 0;
    bool found = false;
    while (!found) {
        long last = -1;
        int checked = 0;
        auto output = [&last, &checked](long x) {
            if (x == last)
                throw false;
            last = x;
            ++checked;
            if (checked == 20)
                throw true;
        };
        assembunny_computer runner(argv[1], output);
        runner.registers[0] = curr;

        try {
            runner.run();
        } catch (bool b) {
            found = b;
            if (found)
                std::cout << curr << std::endl;
        }
        ++curr;
    }

    return 0;
}

#endif // DAY_25
