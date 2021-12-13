#ifndef INTCODE
#define INTCODE

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static const int ADD = 1;
static const int MULTIPLY = 2;
static const int SAVE = 3;
static const int PRINT = 4;
static const int JUMP_TRUE = 5;
static const int JUMP_FALSE = 6;
static const int LESS_THAN = 7;
static const int EQUALS = 8;
static const int MOD_REL = 9;
static const int HALT = 99;

static const std::unordered_set<int> VALID_OPS = {ADD,        MULTIPLY,  SAVE,   PRINT,   JUMP_TRUE,
                                                  JUMP_FALSE, LESS_THAN, EQUALS, MOD_REL, HALT};
static const std::unordered_map<int, std::size_t> NUM_ARGS = {
  {ADD, 3},        {MULTIPLY, 3},  {SAVE, 1},   {PRINT, 1},   {JUMP_TRUE, 2},
  {JUMP_FALSE, 2}, {LESS_THAN, 3}, {EQUALS, 3}, {MOD_REL, 1}, {HALT, 0}};

class intcode_computer {
    public:
    std::unordered_map<std::size_t, long> tape;

    private:
    std::size_t pos;
    long rel_base;

    std::function<long()> input;
    std::function<void(long)> output;
    struct instruction {
        int opcode;
        std::vector<long*> args;
    };

    static int get_opcode(long incode) {
        if (incode < 0)
            throw std::invalid_argument("负数");
        int code = incode % 100;
        if (VALID_OPS.find(code) == VALID_OPS.end())
            throw std::invalid_argument("不支持此运算");
        return code;
    }

    long* get_arg(int mode) {
        long rel_pos;
        switch (mode) {
        case 0:
            return &tape[tape[pos]];

        case 1:
            return &tape[pos];

        case 2:
            rel_pos = tape[pos] + rel_base;
            if (rel_pos < 0)
                throw std::invalid_argument("负数");
            return &tape[rel_pos];

        default:
            throw std::invalid_argument("不支持此指令");
        }
    }

    instruction get_command() {
        instruction output;
        long inst = tape[pos++];
        output.opcode = get_opcode(inst);
        int num_args = NUM_ARGS.at(output.opcode);
        inst /= 100;

        for (int i = 0; i < num_args; ++i) {
            int mode = inst % 10;
            inst /= 10;

            output.args.push_back(get_arg(mode));
            ++pos;
        }

        return output;
    }

    void execute(instruction& command) {
        switch (command.opcode) {
        case ADD:
            *command.args[2] = *command.args[0] + *command.args[1];
            break;

        case MULTIPLY:
            *command.args[2] = *command.args[0] * *command.args[1];
            break;

        case SAVE:
            *command.args[0] = input();
            break;

        case PRINT:
            output(*command.args[0]);
            break;

        case JUMP_TRUE:
            if (*command.args[0])
                pos = *command.args[1];
            break;

        case JUMP_FALSE:
            if (!*command.args[0])
                pos = *command.args[1];
            break;

        case LESS_THAN:
            *command.args[2] = (*command.args[0] < *command.args[1]);
            break;

        case EQUALS:
            *command.args[2] = (*command.args[0] == *command.args[1]);
            break;

        case MOD_REL:
            rel_base += *command.args[0];
            break;
        }
    }

    bool handle_next() {
        instruction command = get_command();
        if (command.opcode == HALT)
            return false;
        execute(command);
        return true;
    }

    public:
    intcode_computer(char* input_file, std::function<long()> input,
                     std::function<void(long)> output) :
        tape(), pos(0), rel_base(0), input(input), output(output) {
        std::ifstream reader(input_file);
        std::string line;

        while (std::getline(reader, line, ',')) {
            tape.insert({tape.size(), std::stol(line)});
        }
    }

    void run() {
        while (handle_next())
            ;
    }
};

#endif // INTCODE
