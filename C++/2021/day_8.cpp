#ifndef DAY_8
#define DAY_8

#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "structures/graph.h"

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::size_t part_1 = 0, part_2 = 0;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream parser(line);
        std::vector<std::string> turned_on(std::istream_iterator<std::string>{parser},
                                           std::istream_iterator<std::string>{});
        bool is_output = false;

        std::size_t output_val = 0;

        std::array<std::unordered_set<char>, 10> digit_map;

        // populate knowns
        for (const std::string& val : turned_on) {
            switch (val.size()) {
            case 2:
                digit_map[1] = std::unordered_set(val.begin(), val.end());
                break;

            case 3:
                digit_map[7] = std::unordered_set(val.begin(), val.end());
                break;

            case 4:
                digit_map[4] = std::unordered_set(val.begin(), val.end());
                break;

            case 7:
                digit_map[8] = std::unordered_set(val.begin(), val.end());
                break;

            default:
                break;
            }
        }

        // 推测：
        for (const std::string& val : turned_on) {
            std::unordered_set lit_on(val.begin(), val.end());
            switch (val.size()) {
            case 6:
                if (std::all_of(digit_map[1].begin(), digit_map[1].end(),
                                [&lit_on](char c) { return lit_on.contains(c); })) {
                    if (std::all_of(digit_map[4].begin(), digit_map[4].end(),
                                    [&lit_on](char c) { return lit_on.contains(c); }))
                        digit_map[9] = lit_on;
                    else
                        digit_map[0] = lit_on;
                } else {
                    digit_map[6] = lit_on;
                }
                break;

            case 5:
                if (std::all_of(digit_map[1].begin(), digit_map[1].end(),
                                [&lit_on](char c) { return lit_on.contains(c); }))
                    digit_map[3] = lit_on;
                else {
                    std::unordered_set top_left = digit_map[4];
                    std::erase_if(top_left,
                                  [&digit_map](char c) { return digit_map[1].contains(c); });
                    if (std::all_of(top_left.begin(), top_left.end(),
                                    [&lit_on](char c) { return lit_on.contains(c); }))
                        digit_map[5] = lit_on;
                    else
                        digit_map[2] = lit_on;
                }
                break;

            default:
                break;
            }
        }

        for (const std::string& val : turned_on) {
            switch (val.size()) {
            case 1:
                is_output = true;
                break;
            case 2:
            case 3:
            case 4:
            case 7:
                if (is_output)
                    ++part_1;
                break;

            default:
                break;
            }

            if (is_output && val.size() != 1) {
                output_val *= 10;
                std::unordered_set lit_on(val.begin(), val.end());

                output_val +=
                  std::find(digit_map.begin(), digit_map.end(), lit_on) - digit_map.begin();
            }
        }

        part_2 += output_val;
    }

    std::cout << part_1 << '\n' << part_2 << std::endl;

    return 0;
}

#endif // DAY_8
