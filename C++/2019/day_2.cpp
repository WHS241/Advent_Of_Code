#ifndef DAY_2
#define DAY_2

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream in_stream(argv[1]);
    std::vector<int> inputs;
    std::string input;

    while (std::getline(in_stream, input, ',')) {
        inputs.push_back(std::stoul(input));
    }

    for (int j = 0; j < 9999; ++j) {
        std::vector<int> copy(inputs);
        try {
            copy[1] = j / 100;
            copy[2] = j % 100;
            for (std::size_t i = 0; 4 * i < inputs.size(); ++i) {
                std::size_t current = 4 * i;
                int opcode = copy[current];

                if (opcode == 99)
                    break;

                int arg1 = copy[current + 1];
                int arg2 = copy[current + 2];
                int target = copy[current + 3];

                if (opcode == 1) {
                    copy[target] = copy[arg1] + copy[arg2];
                } else if (opcode == 2) {
                    copy[target] = copy[arg1] * copy[arg2];
                } else {
                    throw std::domain_error("Bad");
                }
            }

            if (j == 1202)
                std::cout << "Part 1: " << copy[0] << std::endl;

            if (copy[0] == 19690720) {
                std::cout << "Part 2: " << j << std::endl;
                return 0;
            }
        } catch (std::domain_error&) {}
    }

    return 0;
}

#endif // DAY_2
