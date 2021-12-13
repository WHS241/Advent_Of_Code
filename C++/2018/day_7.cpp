#ifndef DAY_7
#define DAY_7 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

int day_7_main(int argc, char** argv) {
#if DAY_7 == 1
    if (argc < 2)
#elif DAY_7 == 2
    if (argc < 4)
#endif
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_map<char, std::list<char>> flow;
    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string s, t, ignore;
        line_read >> ignore >> s >> ignore >> ignore >> ignore >> ignore >> ignore >> t >> ignore
          >> ignore;
        flow[s.front()].push_back(t.front());
    }

    std::set<char> buffer;
    std::unordered_map<char, std::size_t> in_degree;
    for (const auto& p : flow)
        for (const char& s : p.second)
            ++in_degree[s];

    for (const auto& p : flow)
        if (in_degree[p.first] == 0)
            buffer.insert(p.first);

#if DAY_7 == 1
    while (!buffer.empty()) {
        char curr = *buffer.begin();
        buffer.erase(buffer.begin());
        std::cout << curr;

        for (const char& s : flow[curr])
            if (--in_degree[s] == 0)
                buffer.insert(s);
    }

    std::cout << std::endl;
#elif DAY_7 == 2
    std::size_t base = std::stoul(argv[2]);
    std::size_t num_workers = std::stoul(argv[3]);

    std::vector<char> current(num_workers);
    std::list<std::size_t> idle;

    for (std::size_t i = 0; i < num_workers; ++i)
        idle.push_back(i);

    std::size_t time = 0;
    std::vector<std::pair<std::size_t, std::size_t>> times;

    while (!idle.empty() && !buffer.empty()) {
        current[idle.front()] = *buffer.begin();
        times.emplace_back(base + (*buffer.begin() - 'A' + 1), idle.front());
        idle.pop_front();
        buffer.erase(buffer.begin());
    }

    std::make_heap(times.begin(), times.end(), std::greater<>());

    while (!times.empty()) {
        std::pop_heap(times.begin(), times.end(), std::greater<>());
        auto p = times.back();
        times.pop_back();

        time = p.first;
        idle.push_back(p.second);

        for (char c : flow[current[p.second]])
            if (--in_degree[c] == 0)
                buffer.insert(c);

        while (!idle.empty() && !buffer.empty()) {
            current[idle.front()] = *buffer.begin();
            times.emplace_back(time + base + (*buffer.begin() - 'A' + 1), idle.front());
            std::push_heap(times.begin(), times.end(), std::greater<>());
            idle.pop_front();
            buffer.erase(buffer.begin());
        }
    }

    std::cout << time << std::endl;
#endif

    return 0;
}

#endif // DAY_7
