#ifndef DAY_3
#define DAY_3

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>

#include "util/pair_hash.h"

int day_3_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::unordered_set<std::pair<int, int>, util::pair_hash<int, int>> visited_p1, visited_p2;
    std::pair<int, int> curr_p1 = {0, 0}, curr_Santa_p2(curr_p1), curr_Robo_p2(curr_p1);
    visited_p1.insert(curr_p1);
    visited_p2.insert(curr_Santa_p2);
    bool is_first = true;

    for (std::string line; std::getline(reader, line);) {
        for (char c : line) {
            std::pair<int, int>& curr_p2 = is_first ? curr_Santa_p2 : curr_Robo_p2;
            switch (c) {
            case '<':
                --curr_p1.first;
                --curr_p2.first;
                break;

            case '>':
                ++curr_p1.first;
                ++curr_p2.first;
                break;

            case '^':
                ++curr_p1.second;
                ++curr_p2.second;
                break;

            case 'v':
                --curr_p1.second;
                --curr_p2.second;
                break;

            default:
                break;
            }

            visited_p1.insert(curr_p1);
            visited_p2.insert(curr_p2);
            is_first = !is_first;
        }
    }

    std::cout << visited_p1.size() << '\n' << visited_p2.size() << std::endl;

    return 0;
}

#endif // DAY_3
