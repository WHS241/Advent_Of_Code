#ifndef DAY_8
#define DAY_8

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::pair<std::string, long>> instructions;
    for (std::string line; std::getline(reader, line);) {
        instructions.emplace_back();
        std::istringstream line_read(line);
        line_read >> instructions.back().first >> instructions.back().second;
    }

    long acc = 0;

    // Part 1
    std::vector<bool> ran(instructions.size(), false);
    for (std::size_t i = 0; i < instructions.size() && !ran[i];) {
        long del = 1;
        ran[i] = true;
        if (instructions[i].first == "acc")
            acc += instructions[i].second;
        else if (instructions[i].first == "jmp")
            del = instructions[i].second;
        i += del;
    }
    std::cout << acc << '\n';

    // Part 2
    for (std::size_t j = 0; j < instructions.size(); ++j) {
        acc = 0;
        auto copy(instructions);
        if (copy[j].first == "nop")
            copy[j].first = "jmp";
        else if (copy[j].first == "jmp")
            copy[j].first = "nop";
        std::vector<bool> ran(instructions.size(), false);

        std::size_t i = 0;
        while (i < copy.size() && !ran[i]) {
            long del = 1;
            ran[i] = true;
            if (copy[i].first == "acc")
                acc += copy[i].second;
            else if (copy[i].first == "jmp")
                del = copy[i].second;
            i += del;
        }

        if (i >= instructions.size())
            break;
    }
    std::cout << acc << std::endl;

    return 0;
}

#endif // DAY_8
