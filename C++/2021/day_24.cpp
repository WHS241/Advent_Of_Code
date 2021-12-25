#ifndef DAY_24
#define DAY_24

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

/*********************************************************
 * Check codeless solver for explanation of optimization *
 * Is based on nature of input                           *
 *********************************************************/

std::vector<std::pair<std::size_t, int>> calc_restraints(const std::vector<std::string>& monad) {
    std::vector<std::pair<std::size_t, int>> result(14);
    std::vector<std::pair<std::size_t, int>> z_stack;

    for (std::size_t j = 0; j < 14; ++j) {
        bool is_pop = monad[18 * j + 4].back() != '1';
        std::size_t pop_offset, push_offset;
        std::istringstream pop_parser(monad[18 * j + 5]), push_parser(monad[18 * j + 15]);
        std::string ignore;
        pop_parser >> ignore >> ignore >> pop_offset;
        push_parser >> ignore >> ignore >> push_offset;
        if (is_pop) {
            auto [popped_i, popped_off] = z_stack.back();
            z_stack.pop_back();
            result[j] = std::make_pair(popped_i, popped_off + pop_offset);
            result[popped_i] = std::make_pair(j, -result[j].second);
        } else {
            z_stack.emplace_back(j, push_offset);
        }
    }

    return result;
}
bool accept(const std::vector<std::string>& monad, const std::string& model_number) {
    std::size_t i = 0;

    std::unordered_map<char, long> registers;

    for (const std::string& s : monad) {
        std::istringstream parser(s);
        std::string instr, val_1;
        parser >> instr >> val_1;
        if (instr == "inp") {
            registers[val_1.front()] = model_number[i++] - '0';
        } else {
            std::string val_2;
            parser >> val_2;
            std::size_t second_arg =
              std::isalpha(val_2.front()) ? registers[val_2.front()] : std::stol(val_2);
            if (instr == "add")
                registers[val_1.front()] += second_arg;
            else if (instr == "mul")
                registers[val_1.front()] *= second_arg;
            else if (instr == "div")
                registers[val_1.front()] /= second_arg;
            else if (instr == "mod")
                registers[val_1.front()] %= second_arg;
            else if (instr == "eql")
                registers[val_1.front()] = registers[val_1.front()] == second_arg;
        }
    }

    return !registers['z'];
}

int day_24_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> instrs;
    for (std::string s; std::getline(reader, s);) {
        instrs.push_back(s);
    }

    std::vector<std::pair<std::size_t, int>> constraints = calc_restraints(instrs);

    std::string part_1, part_2;
    part_1.resize(14);
    part_2.resize(14);

    for (std::size_t i = 0; i < 14; ++i) {
        if (constraints[i].first > i) {
            if (constraints[i].second < 0) {
                part_1[i] = '9' + constraints[i].second;
                part_1[constraints[i].first] = '9';
                part_2[i] = '1';
                part_2[constraints[i].first] = '1' - constraints[i].second;
            } else {
                part_1[i] = '9';
                part_1[constraints[i].first] = '9' - constraints[i].second;
                part_2[i] = '1' + constraints[i].second;
                part_2[constraints[i].first] = '1';
            }
        }
    }

    if (!accept(instrs, part_1))
        std::cout << "ERROR: " << part_1 << std::endl;
    if (!accept(instrs, part_2))
        std::cout << "ERROR: " << part_2 << std::endl;

    std::cout << part_1 << '\n' << part_2 << std::endl;

    return 0;
}

#endif // DAY_24
