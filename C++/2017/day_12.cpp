#ifndef DAY_12
#define DAY_12 2

#include <graph/components.h>
#include <structures/graph.h>

#include <fstream>
#include <iostream>
#include <sstream>

int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    graph::unweighted_graph<std::size_t, false> g;

    for (std::string line; std::getline(reader, line);) {
        std::size_t v = g.order();
        g.add_vertex(v);
        std::istringstream line_read(line);
        std::string word;
        line_read >> word >> word;

        for (; std::getline(line_read, word, ',');) {
            std::size_t u = std::stoul(word);
            if (u < v)
                g.force_add(u, v);
        }
    }

#if DAY_12 == 1
    std::list<std::unordered_set<std::size_t>> components = graph_alg::connected_components(g);

    for (auto c : components)
        if (c.count(0))
            std::cout << c.size() << std::endl;
#elif DAY_12 == 2
    std::cout << graph_alg::connected_components(g).size() << std::endl;
#endif

    return 0;
}

#endif // DAY_12
