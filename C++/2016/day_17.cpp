#ifndef DAY_17
#define DAY_17

#include <climits>
#include <iostream>
#include <list>
#include <utility>

#include "util.h"

struct state {
    std::string history;
    std::pair<std::size_t, std::size_t> location;
};

constexpr std::size_t MAZE_X = 4;
constexpr std::size_t MAZE_Y = 4;

int day_17_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::string prefix(argv[1]);

    std::list<state> frontier;
    frontier.push_back({"", std::make_pair(0UL, 0UL)});

    bool found_min = false;
    std::size_t max_length = 0;

    while (!frontier.empty()) {
        state current = frontier.front();
        frontier.pop_front();
        if (current.location == std::make_pair(MAZE_X - 1, MAZE_Y - 1)) {
            if (!found_min) {
                std::cout << current.history << '\n';
                found_min = true;
            }
            max_length = std::max(max_length, current.history.size());
            continue;
        }
        std::string options = hex_string_hash(prefix + current.history).substr(0, 4);
        if (options[0] > 'a' && current.location.first != 0)
            frontier.push_back({current.history + "U", std::make_pair(current.location.first - 1,
                                                                      current.location.second)});
        if (options[1] > 'a' && current.location.first != MAZE_X - 1)
            frontier.push_back({current.history + "D", std::make_pair(current.location.first + 1,
                                                                      current.location.second)});
        if (options[2] > 'a' && current.location.second != 0)
            frontier.push_back(
              {current.history + "L",
               std::make_pair(current.location.first, current.location.second - 1)});
        if (options[3] > 'a' && current.location.second != MAZE_X - 1)
            frontier.push_back(
              {current.history + "R",
               std::make_pair(current.location.first, current.location.second + 1)});
    }

    std::cout << max_length << std::endl;

    return 0;
}

#endif // DAY_17
