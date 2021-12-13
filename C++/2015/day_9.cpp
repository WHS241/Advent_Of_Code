#ifndef DAY_9
#define DAY_9

#include "structures/graph.h"

#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    graph::graph<std::string, false, true, std::size_t> dist_graph(graph::adj_matrix);

    std::ifstream reader(argv[1]);
    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string sub;

        line_read >> sub;
        if (!dist_graph.get_translation().count(sub))
            dist_graph.add_vertex(sub);
        std::string end1 = sub;

        line_read >> sub >> sub;
        if (!dist_graph.get_translation().count(sub))
            dist_graph.add_vertex(sub);
        std::string end2 = sub;

        line_read >> sub >> sub;
        dist_graph.set_edge(end1, end2, std::stoul(sub));
    }

    std::size_t min_travel = ULONG_MAX; // PART ONE
    std::size_t max_travel = 0;         // PART_TWO
    std::vector<std::string> path = dist_graph.vertices();
    std::sort(path.begin(), path.end());

    do {
        using namespace std::placeholders;
        std::size_t length = std::transform_reduce(
          path.begin(), path.end() - 1, path.begin() + 1, 0UL, std::plus<>(),
          std::bind(&graph::graph<std::string, false, true, std::size_t>::edge_cost,
                    std::cref(dist_graph), _1, _2));

        min_travel = std::min(min_travel, length); // PART ONE
        max_travel = std::max(max_travel, length); // PART TWO
    } while (std::next_permutation(path.begin(), path.end()));

    std::cout << min_travel << '\n' << max_travel << std::endl;

    return 0;
}

#endif // DAY_9
