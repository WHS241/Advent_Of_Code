#ifndef DAY_15
#define DAY_15 2
#include <cstdlib>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "intcode.cpp"

static constexpr int UP(1), DOWN(2), LEFT(3), RIGHT(4), NUM_MOVES(4);

int day_15_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::unordered_map<int, std::unordered_map<int, int>> cell;
    std::list<int> past_directions;
    std::pair<int, int> position = {0, 0};
    cell[position.first][position.second] = 1;
    std::pair<int, int> ox_pos;

    auto get_opposite = [](int to_reverse) {
        return (to_reverse % 2 == 0) ? to_reverse - 1 : to_reverse + 1;
    };

    auto apply_direction = [](std::pair<int, int> x, int dir) {
        switch (dir) {
        case UP:
            ++x.second;
            break;

        case DOWN:
            --x.second;
            break;

        case LEFT:
            --x.first;
            break;

        case RIGHT:
            ++x.first;
            break;

        default:
            break;
        }

        return x;
    };

    auto find_path = [&cell, &ox_pos, &apply_direction]() {
        std::unordered_map<int, std::unordered_map<int, int>> distance;
        std::pair<int, int> start;
#if DAY_15 == 1
        start = {0, 0};
#elif DAY_15 == 2
        start = ox_pos;
#endif
        distance[start.first][start.second] = 0;
        int max_dist = 0;
        std::list<std::pair<int, int>> frontier;
        frontier.push_back(start);

        while (!frontier.empty()) {
            std::pair<int, int> curr = frontier.front();
            frontier.pop_front();

            std::array<std::pair<int, int>, NUM_MOVES> next;

            for (int i = 1; i <= NUM_MOVES; ++i)
                next[i - 1] = apply_direction(curr, i);

            for (std::pair<int, int>& r : next) {
                if (cell[r.first][r.second]
                    && (distance.find(r.first) == distance.end()
                        || distance[r.first].find(r.second) == distance[r.first].end())) {
                    frontier.push_back(r);
                    max_dist = distance[r.first][r.second] = distance[curr.first][curr.second] + 1;
                }
            }
        }

#if DAY_15 == 1
        std::cout << distance[ox_pos.first][ox_pos.second] << std::endl;
#elif DAY_15 == 2
        std::cout << max_dist << std::endl;
#endif
        std::exit(0);
    };

    past_directions.push_back(-1);
    int next_direction = 1;
    auto input = [&find_path, &next_direction, &cell, &past_directions, &get_opposite,
                  &apply_direction, &position]() {
        while (next_direction <= NUM_MOVES) {
            if (next_direction != get_opposite(past_directions.back())) {
                std::pair<int, int> next_space = apply_direction(position, next_direction);
                if (cell.find(next_space.first) == cell.end()
                    || cell[next_space.first].find(next_space.second)
                         == cell[next_space.first].end())
                    return next_direction;
            }

            ++next_direction;
        }

        if (past_directions.back() != -1) {
            next_direction = get_opposite(past_directions.back());
            return next_direction;
        } else {
            find_path();
            return 1;
        }
    };

    auto output = [&ox_pos, &past_directions, &get_opposite, &next_direction, &apply_direction,
                   &cell, &position](long i) {
        std::pair<int, int> next = apply_direction(position, next_direction);
        cell[next.first][next.second] = i;

        if (i == 2)
            ox_pos = next;

        if (i) {
            position = next;

            if (next_direction == get_opposite(past_directions.back())) {
                next_direction = past_directions.back() + 1;
                past_directions.pop_back();
            } else {
                past_directions.push_back(next_direction);
                next_direction = 1;
            }
        } else {
            ++next_direction;
        }
    };

    intcode_computer runner(argv[1], input, output);

    runner.run();

    return 0;
}

#endif // DAY_15
