#ifndef ASSEMBUNNY
#define ASSEMBUNNY

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

class assembunny_computer {
    public:
    static constexpr std::size_t NUM_REGISTERS = 4;
    std::array<long, NUM_REGISTERS> registers;
    std::vector<std::vector<std::string>> instructions;

    private:
    std::size_t curr;
    std::function<void(long)> output;

    long get_value(const std::string& word) {
        return std::isalpha(word.front()) ? registers[word.front() - 'a'] : std::stol(word);
    }

    public:
    assembunny_computer(char* file, std::function<void(long)> output) :
        instructions(), output(output), curr(0) {
        std::fill(registers.begin(), registers.end(), 0);

        std::ifstream reader(file);

        for (std::string line; std::getline(reader, line);) {
            instructions.emplace_back();
            std::istringstream line_read(line);

            std::copy(std::istream_iterator<std::string>(line_read),
                      std::istream_iterator<std::string>(),
                      std::back_inserter(instructions.back()));
        }
    }

    void run() {
        while (curr < instructions.size()) {
            const std::vector<std::string>& inst = instructions[curr];
            if (inst[0] == "jnz" && get_value(inst[1]))
                curr = (long) curr + get_value(inst[2]);
            else {
                if (inst[0] == "cpy" && inst.size() == 3)
                    registers[inst[2].front() - 'a'] = get_value(inst[1]);
                else if (inst[0] == "inc" && inst.size() == 2)
                    ++registers[inst[1].front() - 'a'];
                else if (inst[0] == "dec" && inst.size() == 2)
                    --registers[inst[1].front() - 'a'];
                else if (inst[0] == "tgl" && inst.size() == 2) {
                    long delta = get_value(inst[1]);
                    if (-delta <= (long) curr && curr + delta < instructions.size()) {
                        std::vector<std::string>& target = instructions[curr + delta];
                        if (target.size() == 2)
                            target[0] = (target[0] == "inc") ? "dec" : "inc";
                        else
                            target[0] = (target[0] == "jnz") ? "cpy" : "jnz";
                    }
                } else if (inst[0] == "out")
                    output(get_value(inst[1]));
                ++curr;
            }
        }
    }
};

#endif // ASSEMBUNNY
