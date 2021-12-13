#include <cstddef>
#ifndef DAY_18
#define DAY_18

#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

int day_18_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t sum_1 = 0, sum_2 = 0;
    for (std::string line; std::getline(reader, line);) {
        std::istringstream parser(line);

        std::vector<std::size_t> p1_operand_stack;
        std::vector<char> p1_op_stack;

        std::vector<std::size_t> p2_operand_stack;
        std::vector<char> p2_op_stack;

        // Shunting yard based approach
        for (std::string val; parser >> val;) {
            std::size_t i = 0;
            while (i < val.size() && val[i] == '(') {
                p1_op_stack.push_back('(');
                p2_op_stack.push_back('(');
                ++i;
            }

            val = val.substr(i);

            if (std::isdigit(val.front())) {
                std::size_t x = std::stoul(val);
                p1_operand_stack.push_back(x);
                p2_operand_stack.push_back(x);

                while (val.back() == ')') {
                    // no precedence in part 1: op stack can't go one further than paren
                    if (p1_op_stack.back() != '(') {
                        std::size_t operand_1 = p1_operand_stack.back();
                        p1_operand_stack.pop_back();
                        std::size_t operand_2 = p1_operand_stack.back();
                        p1_operand_stack.pop_back();

                        // only supporting + and *
                        if (p1_op_stack.back() == '+')
                            p1_operand_stack.push_back(operand_1 + operand_2);
                        else
                            p1_operand_stack.push_back(operand_1 * operand_2);

                        p1_op_stack.pop_back();
                    }

                    while (p2_op_stack.back() != '(') {
                        std::size_t operand_1 = p2_operand_stack.back();
                        p2_operand_stack.pop_back();
                        std::size_t operand_2 = p2_operand_stack.back();
                        p2_operand_stack.pop_back();

                        // only supporting + and *
                        if (p2_op_stack.back() == '+')
                            p2_operand_stack.push_back(operand_1 + operand_2);
                        else
                            p2_operand_stack.push_back(operand_1 * operand_2);

                        p2_op_stack.pop_back();
                    }

                    p1_op_stack.pop_back();
                    p2_op_stack.pop_back();
                    val.pop_back();
                }
            } else {
                if (val.front() == '+') {
                    // Part 1: all equal precedence
                    if (!p1_op_stack.empty() && p1_op_stack.back() != '(') {
                        std::size_t operand_1 = p1_operand_stack.back();
                        p1_operand_stack.pop_back();
                        std::size_t operand_2 = p1_operand_stack.back();
                        p1_operand_stack.pop_back();

                        // only supporting + and *
                        if (p1_op_stack.back() == '+')
                            p1_operand_stack.push_back(operand_1 + operand_2);
                        else
                            p1_operand_stack.push_back(operand_1 * operand_2);

                        p1_op_stack.pop_back();
                    }

                    // Part 2: + has higher precedence
                    while (!p2_op_stack.empty() && p2_op_stack.back() == '+') {
                        std::size_t operand_1 = p2_operand_stack.back();
                        p2_operand_stack.pop_back();
                        std::size_t operand_2 = p2_operand_stack.back();
                        p2_operand_stack.pop_back();
                        p2_operand_stack.push_back(operand_1 + operand_2);
                        p2_op_stack.pop_back();
                    }
                } else {
                    // Part 1: all equal precedence
                    if (!p1_op_stack.empty() && p1_op_stack.back() != '(') {
                        std::size_t operand_1 = p1_operand_stack.back();
                        p1_operand_stack.pop_back();
                        std::size_t operand_2 = p1_operand_stack.back();
                        p1_operand_stack.pop_back();

                        // only supporting + and *
                        if (p1_op_stack.back() == '+')
                            p1_operand_stack.push_back(operand_1 + operand_2);
                        else
                            p1_operand_stack.push_back(operand_1 * operand_2);

                        p1_op_stack.pop_back();
                    }

                    // Part 2: + has higher precedence
                    while (!p2_op_stack.empty() && p2_op_stack.back() != '(') {
                        std::size_t operand_1 = p2_operand_stack.back();
                        p2_operand_stack.pop_back();
                        std::size_t operand_2 = p2_operand_stack.back();
                        p2_operand_stack.pop_back();

                        // only supporting + and *
                        if (p2_op_stack.back() == '+')
                            p2_operand_stack.push_back(operand_1 + operand_2);
                        else
                            p2_operand_stack.push_back(operand_1 * operand_2);

                        p2_op_stack.pop_back();
                    }
                }
                p1_op_stack.push_back(val.front());
                p2_op_stack.push_back(val.front());
            }
        }

        if (!p1_op_stack.empty()) {
            std::size_t operand_1 = p1_operand_stack.back();
            p1_operand_stack.pop_back();
            std::size_t operand_2 = p1_operand_stack.back();
            p1_operand_stack.pop_back();

            // only supporting + and *
            if (p1_op_stack.back() == '+')
                p1_operand_stack.push_back(operand_1 + operand_2);
            else
                p1_operand_stack.push_back(operand_1 * operand_2);

            p1_op_stack.pop_back();
        }

        while (!p2_op_stack.empty()) {
            std::size_t operand_1 = p2_operand_stack.back();
            p2_operand_stack.pop_back();
            std::size_t operand_2 = p2_operand_stack.back();
            p2_operand_stack.pop_back();

            // only supporting + and *
            if (p2_op_stack.back() == '+')
                p2_operand_stack.push_back(operand_1 + operand_2);
            else
                p2_operand_stack.push_back(operand_1 * operand_2);

            p2_op_stack.pop_back();
        }

        sum_1 += p1_operand_stack.front();
        sum_2 += p2_operand_stack.front();
    }

    std::cout << sum_1 << '\n' << sum_2 << std::endl;

    return 0;
}

#endif // DAY_18
