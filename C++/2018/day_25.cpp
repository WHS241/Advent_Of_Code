#ifndef DAY_25
#define DAY_25

#include <fstream>
#include <iostream>

#include <sstream>
#include <structures/graph.h>

#include <graph/components.h>

#include <util/pair_hash.h>

typedef std::tuple<long, long, long, long> coords;

namespace std {
template<> struct hash<coords> {
    std::hash<int> hasher;
    std::size_t operator()(const coords& x) const {
        const auto& [x1, x2, x3, x4] = x;

        std::size_t h = hasher(x1);
        h = util::asym_combine_hash(h, hasher(x2));
        h = util::asym_combine_hash(h, hasher(x3));
        h = util::asym_combine_hash(h, hasher(x4));

        return h;
    }
};
} // namespace std

int day_25_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    graph::unweighted_graph<coords, false> constellations;

    for (std::string line; std::getline(reader, line);) {
        long x, y, z, t;
        std::istringstream parser(line);
        parser >> x;
        parser.ignore(1);
        parser >> y;
        parser.ignore(1);
        parser >> z;
        parser.ignore(1);
        parser >> t;

        coords p(x, y, z, t);
        constellations.add_vertex(p);

        for (const coords& p2 : constellations.vertices()) {
            if (p != p2) {
                auto [x2, y2, z2, t2] = p2;
                if (std::abs(x - x2) + std::abs(y - y2) + std::abs(z - z2) + std::abs(t - t2) <= 3)
                    constellations.force_add(p, p2);
            }
        }
    }

    std::cout << graph_alg::connected_components(constellations).size() << std::endl;

    return 0;
}

#endif // DAY_25
