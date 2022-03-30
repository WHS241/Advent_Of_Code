#ifndef DAY_24
#define DAY_24

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <util/pair_hash.h>

struct state {
    std::vector<bool> visited;
    std::size_t current;
    static constexpr char START = '0';

    std::vector<std::vector<std::size_t>>* g;

    bool is_terminal_part_1() const {
        return std::all_of(visited.begin(), visited.end(), [](bool x) { return x; });
    }

    bool is_terminal_part_2() const {
        return is_terminal_part_1() && !current;
    }

    std::list<std::pair<state, std::size_t>> get_successors() const {
        std::list<std::pair<state, std::size_t>> output;

        if (is_terminal_part_2())
            return output;

        for (std::size_t i = 0; i < visited.size(); ++i) {
            if ((*g)[current][i] != -1UL) {
                state next(*this);
                next.current = i;
                next.visited[i] = true;
                output.emplace_back(next, (*g)[current][i]);
            }
        }

        return output;
    }

    bool operator==(const state& rhs) const {
        return visited == rhs.visited && current == rhs.current;
    }

    bool operator<(const state&) const { return false; }
};

namespace std {
template<> struct hash<state> {
    std::size_t operator()(const state& s) const {
        static std::hash<std::size_t> pos_hasher;
        static std::hash<bool> bool_hasher;
        std::size_t val = pos_hasher(s.current);
        for (bool b : s.visited)
            val = util::asym_combine_hash(val, bool_hasher(b));
        return val;
    }
};
} // namespace std

constexpr char WALL = '#';

int day_24_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::ifstream reader(argv[1]);
    std::vector<std::string> maze;

    std::vector<std::pair<std::size_t, std::size_t>> pos;

    std::size_t x = 0;
    for (std::string line; std::getline(reader, line);) {
        maze.push_back(line);
        std::size_t y = 0;
        for (char c : line) {
            if (std::isdigit(c)) {
                pos.resize(std::max(pos.size(), (std::size_t)(c - '0' + 1)));
                pos[c - '0'] = {x, y};
            }
            ++y;
        }
        ++x;
    }

    // Convert from maze to graph weighted on distance
    std::vector<std::vector<std::size_t>> distances(pos.size(),
                                                    std::vector<std::size_t>(pos.size(), -1UL));
    for (std::size_t i = 0; i < distances.size(); ++i) {
        std::vector<std::vector<bool>> visited(maze.size(),
                                               std::vector<bool>(maze[0].size(), false));
        std::list<std::pair<std::pair<std::size_t, std::size_t>, std::size_t>> frontier;
        frontier.emplace_back(pos[i], 0UL);
        visited[pos[i].first][pos[i].second] = true;

        while (!frontier.empty()) {
            std::pair<std::size_t, std::size_t> next_coords;
            std::size_t dist;
            std::tie(next_coords, dist) = frontier.front();
            frontier.pop_front();

            char curr = maze[next_coords.first][next_coords.second];
            if (std::isdigit(curr) && curr - '0' != i) {
                distances[i][curr - '0'] = dist;
            } else {
                std::array<std::pair<std::size_t, std::size_t>, 4> neighbors = {
                  std::make_pair(next_coords.first - 1, next_coords.second),
                  std::make_pair(next_coords.first + 1, next_coords.second),
                  std::make_pair(next_coords.first, next_coords.second - 1),
                  std::make_pair(next_coords.first, next_coords.second + 1)};
                for (std::pair<std::size_t, std::size_t> p : neighbors) {
                    if (p.first < maze.size() && p.second < maze[0].size()
                        && !visited[p.first][p.second] && maze[p.first][p.second] != WALL) {
                        visited[p.first][p.second] = true;
                        frontier.emplace_back(p, dist + 1);
                    }
                }
            }
        }
    }

    state initial{std::vector<bool>(distances.size(), false), 0UL, &distances};
    initial.visited[0] = true;

    std::vector<std::pair<std::size_t, state>> frontier;
    frontier.emplace_back(0UL, initial);

    std::unordered_set<state> visited;
    bool found_part_1 = false;

    while (!frontier.empty()) {
        std::size_t dist;
        state curr;
        std::pop_heap(frontier.begin(), frontier.end(), std::greater<>());
        std::tie(dist, curr) = frontier.back();
        frontier.pop_back();
        if (visited.count(curr))
            continue;
        visited.insert(curr);

        if (!found_part_1 && curr.is_terminal_part_1()) {
            std::cout << dist << std::endl;
            found_part_1 = true;
        }
        if (curr.is_terminal_part_2()) {
            std::cout << dist << std::endl;
            break;
        }

        for (const std::pair<state, std::size_t>& s : curr.get_successors()) {
            frontier.emplace_back(dist + s.second, s.first);
            std::push_heap(frontier.begin(), frontier.end(), std::greater<>());
        }
    }

    return 0;
}

#endif // DAY_24
