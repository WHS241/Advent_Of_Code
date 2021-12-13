#ifndef DAY_7
#define DAY_7

#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <vector>

#include <graph/search.h>

#include <structures/graph.h>

int day_7_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    // (u, v) = x if color-u bags contain x color-v bags
    graph::graph<std::string, true, true, std::size_t> contains;

    for (std::string line; std::getline(reader, line);) {
        std::size_t pos = line.find("bag");
        std::string curr_color = line.substr(0, pos - 1);
        if (!contains.has_vertex(curr_color))
            contains.add_vertex(curr_color);
        pos = line.find("contain ", pos) + 8;
        line = line.substr(pos);
        std::istringstream inside(line);

        while (inside) {
            std::size_t count;
            std::string color;
            std::string word;
            inside >> word;
            if (std::isdigit(word.front()))
                count = std::stoul(word);
            else
                continue;
            for (std::string word; inside >> word && word.substr(0, 3) != "bag";) {
                if (!color.empty())
                    color.push_back(' ');
                color += word;
            }

            if (!contains.has_vertex(color))
                contains.add_vertex(color);

            contains.force_add(curr_color, color, count);
        }
    }

    std::vector<std::string> order = graph_alg::topological_sort(contains);

    const std::string TARGET = "shiny gold";

    // Part 1
    graph::unweighted_graph<std::string, true> reverse; // reverse all edges
    for (const std::string& s : order)
        reverse.add_vertex(s);
    for (const std::string& u : order)
        for (const std::string& v : contains.neighbors(u))
            reverse.force_add(v, u);
    std::unordered_set<std::string> holds_target;
    for (auto it = order.rbegin(); it != order.rend(); ++it)
        if (holds_target.count(*it) || *it == TARGET)
            for (const std::string& v : reverse.neighbors(*it))
                holds_target.insert(v);
    std::cout << holds_target.size() << '\n';

    // Part 2
    std::unordered_map<std::string, std::size_t> bag_count;
    bag_count[TARGET] = 1;
    for (const std::string& s : order)
        for (const auto& e : contains.edges(s))
            bag_count[e.first] += bag_count[s] * e.second;

    // Don't count the shiny gold itself
    std::cout << std::transform_reduce(
                   bag_count.begin(), bag_count.end(), 0UL, std::plus<std::size_t>(),
                   std::mem_fn(&std::pair<const std::string, std::size_t>::second))
                   - 1
              << std::endl;

    return 0;
}

#endif // DAY_7
