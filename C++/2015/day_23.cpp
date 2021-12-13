#ifndef DAY_23
#define DAY_23 2

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int day_23_main(int argc, char** argv) {
    // [ program name, input file, initial value in register a ]
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<std::string> directions;

    for (std::string line; std::getline(reader, line);)
        directions.push_back(line);

    std::size_t registers[2];
    registers[1] = 0;
    registers[0] = std::stoul(argv[2]);

    for (std::size_t i = 0; i < directions.size();) {
        std::string instruction = directions[i].substr(0, 3);
        if (instruction == "hlf")
            registers[directions[i][4] - 'a'] /= 2;
        else if (instruction == "tpl")
            registers[directions[i][4] - 'a'] *= 3;
        else if (instruction == "inc")
            ++registers[directions[i][4] - 'a'];

        std::size_t next = i + 1;
        if (instruction == "jmp")
            next = (int) i + std::stoi(directions[i].substr(4));
        else if ((instruction == "jie" && !(registers[directions[i][4] - 'a'] % 2))
                 || (instruction == "jio" && (registers[directions[i][4] - 'a'] == 1)))
            next = (int) i + std::stoi(directions[i].substr(7));
        i = next;
    }

    std::cout << registers[0] << ' ' << registers[1] << std::endl;

    return 0;
}

#endif // DAY_23
