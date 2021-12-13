#ifndef DAY_6
#define DAY_6 2

#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

int day_6_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::ifstream in_read(argv[1]);
    std::string line;

    static const char DELIM = ')';

    std::unordered_map<std::string, std::string> g;

    while (std::getline(in_read, line)) {
        std::size_t split = line.find(DELIM);
        std::string parent = line.substr(0, split);
        std::string child = line.substr(split + 1);
        g[child] = parent;
    }

#if DAY_6 == 1
    std::unordered_map<std::string, std::list<std::string>> reverse_map;
    for (const std::pair<const std::string, std::string>& p : g)
        reverse_map[p.second].push_back(p.first);
    std::string root("COM");

    std::list<std::string> next_queue;
    next_queue.push_back(root);

    std::unordered_map<std::string, std::size_t> depths;
    while (!next_queue.empty()) {
        std::string next = next_queue.front();
        next_queue.pop_front();

        for (const std::string& s : reverse_map[next])
            depths[s] = depths[next] + 1;
    }

    std::cout << std::transform_reduce(
      depths.begin(), depths.end(), std::size_t(0), std::plus<>(),
      std::mem_fn(&std::pair<const std::string, std::size_t>::second))
              << std::endl;
#elif DAY_6 == 2
    std::string start("YOU"), end("SAN");
    std::list<std::string> start_to_center, end_to_center;
    start_to_center.push_front(start);
    end_to_center.push_front(end);

    // 找出整个path to root
    while (g.find(start_to_center.front()) != g.end())
        start_to_center.push_front(g[start_to_center.front()]);
    while (g.find(end_to_center.front()) != g.end())
        end_to_center.push_front(g[end_to_center.front()]);

    // 找lowest common ancestor
    while (!start_to_center.empty() && !end_to_center.empty()
           && start_to_center.front() == end_to_center.front()) {
        start_to_center.pop_front();
        end_to_center.pop_front();
    }

    // 去掉LCA
    start_to_center.pop_back();
    end_to_center.pop_back();

    std::cout << start_to_center.size() + end_to_center.size() << std::endl;
#endif

    return 0;
}

#endif // DAY_6
