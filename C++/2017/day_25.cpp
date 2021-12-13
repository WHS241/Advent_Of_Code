#ifndef DAY_25
#define DAY_25

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

struct transition {
    bool write;
    bool move_right;
    std::size_t next_state;
};

int day_25_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    
    std::string line;
    std::string word;

    std::vector<std::array<transition, 2>> model;

    std::getline(reader, line);
    std::size_t state = line[line.size() - 2] - 'A';
    std::size_t steps;

    std::getline(reader, line);
    {
        std::istringstream line_read(line);
        line_read >> word >> word >> word >> word >> word >> word;
        steps = std::stoul(word);
    }

    std::getline(reader, line);

    while (std::getline(reader, line)) {
        model.emplace_back();
        for (std::size_t i = 0; i < 2; ++i) {
            std::getline(reader, line);
            std::size_t pos = line[line.size() - 2] - '0';

            std::getline(reader, line);
            model.back()[pos].write = line[line.size() - 2] - '0';
            std::getline(reader, line);
            model.back()[pos].move_right = line.find('r') != std::string::npos;
            std::getline(reader, line);
            model.back()[pos].next_state = line[line.size() - 2] - 'A';
        }

        std::getline(reader, line);
    }

    std::list<bool> tape{false};

    auto it = tape.begin();
    for (std::size_t i = 0; i < steps; ++i) {
        const transition& t = model[state][*it];
        *it = t.write;
        state = t.next_state;
        if (t.move_right) {
            ++it;
            if (it == tape.end()) {
                tape.push_back(false);
                --it;
            }
        } else {
            if (it == tape.begin())
                tape.push_front(false);
            --it;
        }
    }

    std::cout << std::count(tape.begin(), tape.end(), true) << std::endl;

    return 0;
}

#endif // DAY_25
