#ifndef DAY_10
#define DAY_10

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <sequence/order_stats.h>

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> input(std::istream_iterator<std::string>{reader},
                                   std::istream_iterator<std::string>{});

    std::size_t part_1 = 0;
    std::vector<std::size_t> part_2_scores;
    for (const std::string& s : input) {
        std::vector<char> chunk_stack;
        for (char c : s) {
            if (c == '(' || c == '[' || c == '{' || c == '<') {
                chunk_stack.push_back(c);
            } else if (c - chunk_stack.back() != 1 && c - chunk_stack.back() != 2) {
                switch (c) {
                case ')':
                    part_1 += 3;
                    break;

                case ']':
                    part_1 += 57;
                    break;

                case '}':
                    part_1 += 1197;
                    break;

                case '>':
                    part_1 += 25137;
                    break;

                default:
                    std::cout << "不支持" << c << std::endl;
                    return 1;
                }
                chunk_stack.clear();
                break;
            } else {
                chunk_stack.pop_back();
            }
        }

        if (!chunk_stack.empty()) {
            std::string rem(chunk_stack.rbegin(), chunk_stack.rend());
            for (char& c : rem) {
                switch (c) {
                case '(':
                    c = '1';
                    break;

                case '[':
                    c = '2';
                    break;

                case '{':
                    c = '3';
                    break;

                case '<':
                    c = '4';
                    break;
                }
            }
            part_2_scores.push_back(std::stoul(rem, nullptr, 5));
        }
    }

    std::cout << part_1 << '\n'
              << sequence::selection(part_2_scores.begin(), part_2_scores.end(),
                                     part_2_scores.size() / 2)
              << std::endl;

    return 0;
}

#endif // DAY_10
