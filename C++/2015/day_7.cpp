#ifndef DAY_7
#define DAY_7

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <unordered_map>
#include <vector>

int day_7_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_map<std::string, std::string> instruction_map; // {wire : how to set}
    std::string sep = " -> ";

    for (std::string line; std::getline(reader, line);) {
        std::size_t sep_pos = line.find(sep);
        instruction_map[line.substr(sep_pos + sep.size())] = line.substr(0, sep_pos);
    }

    // graph critical path-like approach
    std::unordered_map<std::string, std::list<std::string>> dependencies;
    std::unordered_map<std::string, std::size_t> unfulfilled_prereqs; // simple count
    std::list<std::string> ready;
    for (const std::pair<const std::string, std::string>& p : instruction_map) {
        std::istringstream reader(p.second);
        std::string substr;
        reader >> substr;
        // handle LHS of bitwise op, if exists
        if (std::islower(substr[0])) {
            dependencies[substr].push_back(p.first);
            ++unfulfilled_prereqs[p.first];
            reader >> substr;
        } else if (std::isdigit(substr[0])) {
            reader >> substr;
        }
        reader >> substr;
        if (std::isalpha(substr[0])) {
            dependencies[substr].push_back(p.first);
            ++unfulfilled_prereqs[p.first];
        }

        if (unfulfilled_prereqs[p.first] == 0)
            ready.push_back(p.first);
    }

    std::unordered_map<std::string, uint16_t> values;

    while (!ready.empty()) {
        std::string curr = ready.front();
        ready.pop_front();

        std::istringstream curr_reader(instruction_map[curr]);
        std::vector<std::string> parsed(std::istream_iterator<std::string>{curr_reader},
                                        std::istream_iterator<std::string>{});

        if (parsed.size() == 1) {
            values[curr] =
              std::isalpha(parsed[0][0]) ? values[parsed[0]] : (uint16_t) std::stoi(parsed[0]);
        } else if (parsed.size() == 2) { // unary op, only NOT is supported
            uint16_t input =
              std::isalpha(parsed[1][0]) ? values[parsed[1]] : (uint16_t) std::stoi(parsed[1]);
            values[curr] = ~input;
        } else if (parsed.size() == 3) { // binary op
            uint16_t inputs[2];
            inputs[0] =
              std::isalpha(parsed[0][0]) ? values[parsed[0]] : (uint16_t) std::stoi(parsed[0]);
            inputs[1] =
              std::isalpha(parsed[2][0]) ? values[parsed[2]] : (uint16_t) std::stoi(parsed[2]);

            if (parsed[1] == "AND")
                values[curr] = inputs[0] & inputs[1];
            else if (parsed[1] == "OR")
                values[curr] = inputs[0] | inputs[1];
            else if (parsed[1] == "LSHIFT")
                values[curr] = inputs[0] << inputs[1];
            else if (parsed[1] == "RSHIFT")
                values[curr] = inputs[0] >> inputs[1];
        }

        for (const std::string& d : dependencies[curr]) {
            --unfulfilled_prereqs[d];
            if (!unfulfilled_prereqs[d])
                ready.push_back(d);
        }
    }

    for (const std::pair<const std::string, uint16_t>& p : values) {
        std::cout << p.first << ": " << p.second << std::endl;
    }

    return 0;
}

#endif // DAY_7
