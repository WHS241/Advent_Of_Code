#ifndef DAY_13
#define DAY_13

#define DAY_13_PART 2

#include <climits>
#include <fstream>
#include <iostream>

#include "structures/graph.h"

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    graph::graph<std::string, false, true, int> input;

    for (std::string line; std::getline(reader, line);) {
        std::string v1, v2, weight;
        std::istringstream line_read(line);
        line_read >> v1 >> weight >> v2;
        if (!input.get_translation().count(v1))
            input.add_vertex(v1);
        if (!input.get_translation().count(v2))
            input.add_vertex(v2);
        input.set_edge(v1, v2,
                       std::stoi(weight) + (input.has_edge(v1, v2) ? input.edge_cost(v1, v2) : 0));
    }

#if DAY_13_PART == 2
    input.add_vertex("");
    for (const std::string& s : input.vertices())
        if (s != "")
            input.set_edge("", s, 0);
#endif

    std::vector<std::string> verts = input.vertices();
    std::sort(verts.begin(), verts.end());

    int max_happiness = INT_MIN;

    do {
        int current = 0;
        for (std::size_t i = 0; i < verts.size(); ++i)
            current += input.edge_cost(verts[i], verts[(i + 1) % verts.size()]);
        max_happiness = std::max(current, max_happiness);
    } while (std::next_permutation(verts.begin(), verts.end()));

    std::cout << max_happiness << std::endl;

    return 0;
}

#endif // DAY_13
