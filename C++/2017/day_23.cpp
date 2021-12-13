#ifndef DAY_23
#define DAY_23 2

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <vector>

long get_value(std::unordered_map<char, long>& registers, const std::string& s) {
    return std::isalpha(s.front()) ? registers[s.front()] : std::stoi(s);
}

int day_23_main(int argc, char** argv) {
#if DAY_23==1
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::vector<std::string>> instructions;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);

        instructions.emplace_back();
        std::copy(std::istream_iterator<std::string>(line_read),
                  std::istream_iterator<std::string>(), std::back_inserter(instructions.back()));
    }

    std::unordered_map<char, long> registers;

    std::size_t num_mult = 0;

    for (std::size_t i = 0; i < instructions.size();) {
        if (instructions[i].front() == "set") {
            registers[instructions[i][1][0]] = get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "mul") {
            registers[instructions[i][1][0]] *= get_value(registers, instructions[i][2]);
            ++num_mult;
            ++i;
        } else if (instructions[i].front() == "sub") {
            registers[instructions[i][1][0]] -= get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "jnz") {
            bool jump = get_value(registers, instructions[i][1]) != 0;
            if (jump)
                i += get_value(registers, instructions[i][2]);
            else
                ++i;
        }
    }

    std::cout << num_mult << std::endl;
#elif DAY_23==2
    // Analyzed/optimized program
    std::size_t b = 84 * 100 + 100000;
    std::size_t c = b + 17000;

    std::size_t h = 0;
    while (b <= c) {
        for (std::size_t i = 2; i * i <= b; ++i) {
            if (b % i == 0) {
                ++h;
                break;
            }
        }

        b += 17;
    }
    std::cout << h << std::endl;
#endif
    
    return 0;
}

#endif // DAY_23
