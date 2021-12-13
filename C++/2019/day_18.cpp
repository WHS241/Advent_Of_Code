#ifndef DAY_18
#define DAY_18

#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "structures/graph.h"
#include "structures/heap"
#include "util/pair_hash.h"

class state {
    public:
    std::vector<bool> visited;
    std::vector<char> current;
    static constexpr char START = '@';

    graph::graph<char, false, true, std::size_t>* g;

    state(const std::vector<bool>& visited, const std::vector<char>& current,
          graph::graph<char, false, true, std::size_t>* g) :
        visited(visited), current(current), g(g) {}

    bool is_terminal() const {
        return std::all_of(visited.begin(), visited.end(), [](bool x) { return x; });
    }

    std::list<std::pair<state, std::size_t>> get_successors() const {
        std::list<std::pair<state, std::size_t>> output;

        if (is_terminal())
            return output;

        for (std::size_t i = 0; i < current.size(); ++i) {
            char c = current[i];
            for (std::pair<char, std::size_t> e : g->edges(c)) {
                if (std::isdigit(e.first) || !std::isupper(e.first)
                    || visited[std::tolower(e.first) - 'a']) {
                    state next(*this);
                    next.current[i] = e.first;
                    if (std::islower(e.first))
                        next.visited[e.first - 'a'] = true;
                    output.emplace_back(next, e.second);
                }
            }
        }

        return output;
    }

    bool operator==(const state& rhs) const {
        return visited == rhs.visited
            && std::is_permutation(current.begin(), current.end(), rhs.current.begin());
    }
};

namespace std {
template<> struct hash<state> {
    std::size_t operator()(const state& s) const {
        std::hash<char> char_hasher;
        std::hash<bool> bool_hasher;
        std::size_t val = char_hasher(s.current[0]);
        for (std::size_t i = 1; i < s.current.size(); ++i)
            val = util::asym_combine_hash(val, char_hasher(s.current[i]));
        for (bool b : s.visited)
            val = util::asym_combine_hash(val, bool_hasher(b));

        return val;
    }
};
} // namespace std

constexpr char WALL = '#';

int day_18_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::vector<std::vector<char>> map;
    std::ifstream reader(argv[1]);

    for (std::string line; std::getline(reader, line);)
        map.emplace_back(line.begin(), line.end());

    std::unordered_map<char, std::pair<std::size_t, std::size_t>> targets;

    char start_pos = '0';

    for (std::size_t i = 0; i < map.size(); ++i) {
        for (std::size_t j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == state::START)
                map[i][j] = start_pos++;
            if (std::isalnum(map[i][j])) {
                targets.emplace(map[i][j], std::make_pair(i, j));
            }
        }
    }

    std::size_t search_state_vec_size =
      std::max_element(targets.begin(), targets.end(),
                       [](auto x, auto y) { return x.first < y.first; })
        ->first
      - 'a' + 1;

    graph::graph<char, false, true, std::size_t> search_graph(graph::adj_matrix);
    for (auto c : targets)
        search_graph.add_vertex(c.first);

    for (auto c : targets) {
        // BFS to find the shortest unobstructed path to another target
        char start = c.first;
        std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
        std::list<std::pair<std::pair<std::size_t, std::size_t>, std::size_t>> frontier;
        frontier.push_back(std::make_pair(c.second, 0U));
        visited[c.second.first][c.second.second] = true;

        while (!frontier.empty()) {
            std::pair<std::size_t, std::size_t> next_coords;
            std::size_t dist;
            std::tie(next_coords, dist) = frontier.front();
            frontier.pop_front();

            char next_space = map[next_coords.first][next_coords.second];

            if (next_space != start && (next_space == state::START || std::isalpha(next_space))) {
                search_graph.set_edge(start, next_space, dist);
            } else {
                std::vector<std::pair<std::size_t, std::size_t>> to_add;
                if (next_coords.first != 0)
                    to_add.emplace_back(next_coords.first - 1, next_coords.second);
                if (next_coords.first != visited.size() - 1)
                    to_add.emplace_back(next_coords.first + 1, next_coords.second);
                if (next_coords.second != 0)
                    to_add.emplace_back(next_coords.first, next_coords.second - 1);
                if (next_coords.second != visited[0].size() - 1)
                    to_add.emplace_back(next_coords.first, next_coords.second + 1);

                for (std::pair<std::size_t, std::size_t> pos : to_add) {
                    if (map[pos.first][pos.second] != WALL && !visited[pos.first][pos.second]) {
                        visited[pos.first][pos.second] = true;
                        frontier.push_back(std::make_pair(pos, dist + 1));
                    }
                }
            }
        }
    }

    std::vector<bool> initial_state_info(search_state_vec_size, true);
    for (char c : search_graph.vertices())
        if (std::islower(c))
            initial_state_info[c - 'a'] = false;

    std::vector<char> start_spaces;
    for (char c = '0'; c < start_pos; ++c)
        start_spaces.push_back(c);

    state initial_state(initial_state_info, start_spaces, &search_graph);

    auto compare = [](const std::pair<state, std::size_t>& x,
                      const std::pair<state, std::size_t>& y) {
        return x.second < y.second;
    };

    std::cout << search_graph << std::endl;

    heap::Fibonacci<std::pair<state, std::size_t>, decltype(compare)> frontier(compare);
    frontier.add(std::make_pair(initial_state, 0));

    std::unordered_set<state> visited;

    while (!(&frontier)->empty()) {
        std::pair<state, std::size_t> next = frontier.remove_root();
        if (visited.find(next.first) == visited.end()) {
            visited.insert(next.first);
        } else {
            continue;
        }

        if (next.first.is_terminal()) {
            std::cout << next.second << std::endl;
            break;
        }

        for (const std::pair<state, std::size_t>& s : next.first.get_successors()) {
            frontier.add(std::make_pair(s.first, s.second + next.second));
        }
    }

    return 0;
}

#endif // DAY_18
