#ifndef DAY_21
#define DAY_21 2

#include <fstream>
#include <iostream>
#include <numeric>

#include <unordered_set>
#include <util/pair_hash.h>

#include "day_19.cpp"

int day_21_main(int argc, char** argv) {
    /*
        if (argc < 3)
            return 1;

        std::ifstream reader(argv[1]);

        std::vector<std::tuple<std::string, std::size_t, std::size_t, std::size_t>> code;
        std::array<std::size_t, 6> registers;
        std::fill(registers.begin(), registers.end(), 0UL);
        std::size_t* ip;

        registers[0] = std::stoul(argv[2]);

        std::string line;
        std::getline(reader, line);
        ip = registers.data() + std::stoul(line.substr(4));

        while (std::getline(reader, line)) {
            std::istringstream parser(line);
            std::string instr;
            std::size_t a, b, c;
            parser >> instr >> a >> b >> c;
            code.emplace_back(instr, a, b, c);
        }

        std::unordered_set<std::size_t> halts;
        std::size_t num_exec = 0;
        while (*ip < code.size()) {
            if (*ip == 28) {
                if (halts.contains(registers[4]))
                    break;
                std::cout << registers[4] << ' ' << std::flush;
                halts.insert(registers[4]);
            }
            auto [instr, a, b, c] = code[*ip];
            registers[c] = (functions.at(instr))(a, b, registers);
            ++*ip;
            ++num_exec;
        }

        std::cout << num_exec << std::endl;
    */

    std::size_t x = 0;
    std::unordered_set<std::size_t> encountered;

    std::size_t last;

    while (true) {
        std::size_t y = x | 65536;
        x = 10552971;

        while (y != 0) {
            x = (x + y % 256) % 16777216;
            x = (x * 65899) % 16777216;
            y /= 256;
        }

        if (encountered.empty()) {
            std::cout << x << std::endl;
        } else if (encountered.contains(x))
            break;

        encountered.insert(x);
        last = x;
    }

    std::cout << last << std::endl;

    return 0;
}

#endif // DAY_21
