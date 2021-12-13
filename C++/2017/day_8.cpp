#ifndef DAY_8
#define DAY_8

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::unordered_map<std::string, long> registers;

    auto get = [&registers](const std::string& s) {
        return std::isalpha(s.front()) ? registers[s] : std::stol(s);
    };

    std::ifstream reader(argv[1]);

    long max = 0;
    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string reg, instruction, check1, op, check2;
        long change_v, check1_v, check2_v;

        line_read >> reg >> instruction >> change_v >> check1 >> check1 >> op >> check2;

        check1_v = get(check1);
        check2_v = get(check2);

        bool pass;

        if (op == ">")
            pass = check1_v > check2_v;
        else if (op == "<")
            pass = check1_v < check2_v;
        else if (op == ">=")
            pass = check1_v >= check2_v;
        else if (op == "<=")
            pass = check1_v <= check2_v;
        else if (op == "==")
            pass = check1_v == check2_v;
        else if (op == "!=")
            pass = check1_v != check2_v;

        if (pass) {
            if (instruction == "inc")
                registers[reg] += change_v;
            else
                registers[reg] -= change_v;
            max = std::max(max, registers[reg]);
        }
    }

    for (const std::pair<const std::string, long>& p : registers)
        std::cout << p.first << ": " << p.second << std::endl;

    std::cout << "MAX: " << max << std::endl;

    return 0;
}

#endif // DAY_8
