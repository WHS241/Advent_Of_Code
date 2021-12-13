#ifndef DAY_21
#define DAY_21 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <tuple>
#include <vector>

std::vector<std::size_t> scramble(std::vector<std::size_t> initial,
                                  const std::vector<std::string>& instructions) {
    std::vector<std::size_t> lookup(initial.size());
    for (std::size_t i = 0; i < initial.size(); ++i)
        lookup[initial[i]] = i;
    for (const std::string& line : instructions) {
        std::istringstream line_read(line);
        std::string word;
        std::string ignore;
        line_read >> word;
        if (word == "swap") {
            line_read >> word;

            if (word == "position") {
                std::size_t x, y;
                line_read >> x >> ignore >> ignore >> y;
                std::swap(initial.at(x), initial.at(y));
                std::swap(lookup[initial[x]], lookup[initial[y]]);
            } else {
                char x, y;
                line_read >> x >> ignore >> ignore >> y;
                std::swap(lookup.at(x - 'a'), lookup.at(y - 'a'));
                std::swap(initial[lookup[x - 'a']], initial[lookup[y - 'a']]);
            }
        } else if (word == "rotate") {
            line_read >> word;
            if (word == "based") {
                char pivot;
                line_read >> ignore >> ignore >> ignore >> ignore >> pivot;
                std::size_t pivot_i = lookup[pivot - 'a'];
                if (pivot_i >= 4)
                    ++pivot_i;
                std::rotate(initial.rbegin(), initial.rbegin() + (pivot_i + 1) % initial.size(),
                            initial.rend());
            } else {
                std::size_t steps;
                line_read >> steps;
                if (word == "left")
                    std::rotate(initial.begin(), initial.begin() + steps % initial.size(),
                                initial.end());
                else
                    std::rotate(initial.rbegin(), initial.rbegin() + steps % initial.size(),
                                initial.rend());
            }
            for (std::size_t i = 0; i < initial.size(); ++i)
                lookup[initial[i]] = i;
        } else {
            std::size_t x, y;
            line_read >> ignore >> x >> ignore;
            if (word == "move")
                line_read >> ignore;
            line_read >> y;
            if (word == "reverse") {
                std::reverse(initial.begin() + x, initial.begin() + y + 1);
                for (std::size_t i = x; i <= y; ++i)
                    lookup[initial[i]] = i;
            } else {
                bool is_left = x < y;
                if (is_left) {
                    std::rotate(initial.begin() + x, initial.begin() + x + 1,
                                initial.begin() + y + 1);
                    for (std::size_t i = x; i <= y; ++i)
                        lookup[initial[i]] = i;
                } else {
                    std::rotate(initial.rbegin() + (initial.size() - x - 1),
                                initial.rbegin() + (initial.size() - x),
                                initial.rbegin() + (initial.size() - y));
                    for (std::size_t i = x; i != -1 && i >= y; --i)
                        lookup[initial[i]] = i;
                }
            }
        }
    }

    return initial;
}

std::vector<std::size_t> unscramble(std::vector<std::size_t> initial,
                                    const std::vector<std::string>& instructions) {
    std::vector<std::string> reversed_instructions;
    for (auto it = instructions.rbegin(); it != instructions.rend(); ++it) {
        std::string step = *it;
        std::string verb = step.substr(0, 4);
        if (verb == "swap" || verb == "reve")
            reversed_instructions.push_back(step);
        else if (verb == "move") {
            std::size_t delims[3];
            delims[0] = 4;
            delims[1] = step.find("to") - 1;
            delims[2] = step.find(" pos", delims[1]);
            reversed_instructions.push_back(verb + step.substr(delims[2])
                                            + step.substr(delims[1], delims[2] - delims[1])
                                            + step.substr(delims[0], delims[1] - delims[0]));
        } else if (step[7] == 'l')
            reversed_instructions.push_back(step.substr(0, 7) + "right" + step.substr(11));
        else if (step[7] == 'r')
            reversed_instructions.push_back(step.substr(0, 7) + "left" + step.substr(12));
        else {
            // rotate based on letter
            initial = scramble(initial, reversed_instructions);
            reversed_instructions.clear();

            // This only works on passwords of size 8
            char target = step.back();
            std::size_t curr_index =
              std::find(initial.begin(), initial.end(), target - 'a') - initial.begin();
            if (!curr_index)
                curr_index = initial.size();
            reversed_instructions.push_back(
              "rotate left "
              + std::to_string(curr_index % 2
                                 ? (curr_index + 1) / 2
                                 : ((curr_index + initial.size()) / 2 + 1) % initial.size())
              + "steps");
        }
    }

    return scramble(initial, reversed_instructions);
}

int day_21_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
#if DAY_21 == 1
    std::size_t password_length = std::stoul(argv[2]);

    std::vector<std::size_t> password(password_length);
    std::iota(password.begin(), password.end(), 0UL);

    std::vector<std::string> instructions;

    for (std::string line; std::getline(reader, line);) {
        instructions.push_back(line);
    }

    password = scramble(password, instructions);
#elif DAY_21 == 2
    std::string scrambled(argv[2]);

    std::vector<std::string> instructions;

    for (std::string line; std::getline(reader, line);)
        instructions.push_back(line);
    std::vector<std::size_t> password(scrambled.size());
    std::transform(scrambled.begin(), scrambled.end(), password.begin(),
                   [](char c) -> std::size_t { return c - 'a'; });

    password = unscramble(password, instructions);
#endif

    std::transform(password.begin(), password.end(), std::ostreambuf_iterator<char>(std::cout),
                   [](std::size_t x) -> char { return x + 'a'; });
    std::cout << std::endl;

    return 0;
}

#endif // DAY_21
