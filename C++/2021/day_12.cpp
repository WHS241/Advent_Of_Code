#ifndef DAY_12
#define DAY_12

#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

#include <structures/graph.h>

void filtered_dfs(const graph::unweighted_graph<std::string, false>& caves,
                  std::list<std::string>& history,
                  std::unordered_map<std::string, std::size_t>& num_paths) {
    for (const std::string& s : caves.neighbors(history.back())) {
        if (std::islower(s.front())) {
            ++num_paths[s];
        } else if (std::isupper(s.front())) {
            history.push_back(s);
            filtered_dfs(caves, history, num_paths);
            history.pop_back();
        }
    }
}

std::size_t part_1_dfs(const graph::graph<std::string, false, true, std::size_t>& small_caves,
                       std::list<std::string>& history, std::unordered_set<std::string>& visited,
                       const std::string& end) {
    if (history.back() == end) {
        std::size_t result = 1;
        for (auto it = ++history.begin(); it != history.end(); ++it) {
            auto it2 = it;
            --it2;
            result *= small_caves.edge_cost(*it2, *it);
        }
        return result;
    }
    std::size_t result = 0;
    for (const std::string& s : small_caves.neighbors(history.back())) {
        if (!visited.contains(s)) {
            visited.insert(s);
            history.push_back(s);
            result += part_1_dfs(small_caves, history, visited, end);
            history.pop_back();
            visited.erase(s);
        }
    }
    return result;
}

std::size_t part_2_dfs(const graph::graph<std::string, false, true, std::size_t>& small_caves,
                       std::list<std::string>& history, std::unordered_set<std::string>& visited,
                       const std::string& end, bool repeated,
                       const std::unordered_map<std::string, std::size_t>& self_loops) {
    if (history.back() == end) {
        std::size_t result = 1;
        for (auto it = ++history.begin(); it != history.end(); ++it) {
            auto it2 = it;
            --it2;
            result *= (*it == *it2) ? self_loops.at(*it) : small_caves.edge_cost(*it2, *it);
        }
        return result;
    }
    std::size_t result = 0;
    for (const std::string& s : small_caves.neighbors(history.back())) {
        if (!visited.contains(s)) {
            visited.insert(s);
            history.push_back(s);
            result += part_2_dfs(small_caves, history, visited, end, repeated, self_loops);
            history.pop_back();
            visited.erase(s);
        } else if (s != history.front() && !repeated) {
            history.push_back(s);
            result += part_2_dfs(small_caves, history, visited, end, true, self_loops);
            history.pop_back();
        }
    }
    if (!repeated && history.size() > 1 && self_loops.contains(history.back())
        && self_loops.at(history.back()) != 0) {
        history.push_back(history.back());
        result += part_2_dfs(small_caves, history, visited, end, true, self_loops);
        history.pop_back();
    }
    return result;
}
int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    graph::unweighted_graph<std::string, false> caves;

    for (std::string s; std::getline(reader, s);) {
        std::size_t index = s.find('-');
        std::string v1 = s.substr(0, index);
        std::string v2 = s.substr(index + 1);

        if (!caves.has_vertex(v1))
            caves.add_vertex(v1);
        if (!caves.has_vertex(v2))
            caves.add_vertex(v2);

        caves.force_add(v1, v2);
    }

    // small caves only
    graph::graph<std::string, false, true, std::size_t> small_caves_reduction;
    std::unordered_map<std::string, std::size_t> self_loops;
    for (const std::string& s : caves.vertices())
        if (std::islower(s.front()))
            small_caves_reduction.add_vertex(s);
    for (const std::string& s : small_caves_reduction.vertices()) {
        std::list<std::string> history;
        std::unordered_map<std::string, std::size_t> num_paths;
        history.push_back(s);
        filtered_dfs(caves, history, num_paths);
        for (const auto& p : num_paths) {
            if (s < p.first)
                small_caves_reduction.force_add(s, p.first, p.second);
            else if (s == p.first)
                self_loops[s] = p.second;
        }
    }

    std::list<std::string> path;
    std::unordered_set<std::string> history;
    path.push_back("start");
    history.insert("start");
    std::size_t part_1 = part_1_dfs(small_caves_reduction, path, history, "end");
    std::cout << part_1 << '\n';

    std::size_t part_2 = part_2_dfs(small_caves_reduction, path, history, "end", false, self_loops);
    std::cout << part_2 << std::endl;

    return 0;
}

#endif // DAY_12
