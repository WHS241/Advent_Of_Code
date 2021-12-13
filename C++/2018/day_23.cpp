#ifndef DAY_23
#define DAY_23 2

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <tuple>
#include <unordered_set>

#include <structures/graph.h>

#include <util/pair_hash.h>

//#include <linear_solver/linear_solver.h>

typedef std::tuple<int, int, int> point;        // {x, y, z}
typedef std::tuple<int, int, int, int> nanobot; // {point, range}

namespace std {
template<> struct hash<nanobot> {
    std::hash<int> hasher;
    std::size_t operator()(const nanobot& x) const {
        const auto& [x1, x2, x3, x4] = x;

        std::size_t h = hasher(x1);
        h = util::asym_combine_hash(h, hasher(x2));
        h = util::asym_combine_hash(h, hasher(x3));
        h = util::asym_combine_hash(h, hasher(x4));

        return h;
    }
};
template<> struct hash<point> {
    std::hash<int> hasher;
    std::size_t operator()(const point& x) const {
        const auto& [x1, x2, x3] = x;

        std::size_t h = hasher(x1);
        h = util::asym_combine_hash(h, hasher(x2));
        h = util::asym_combine_hash(h, hasher(x3));

        return h;
    }
};
} // namespace std

// max clique via Bron Kerbosch
void Bron_Kerbosch(std::list<std::unordered_set<nanobot>>& max_cliques,
                   const graph::unweighted_graph<nanobot, false>& g,
                   const std::unordered_set<nanobot>& curr, std::unordered_set<nanobot>& candidates,
                   std::unordered_set<nanobot>& excluded) {
    if (!max_cliques.empty()
        && curr.size() + candidates.size()
             < max_cliques.back()
                 .size()) // no max clique can come from this branch; can prune the search tree
        return;
    if (candidates.empty() && excluded.empty()) {
        if (!max_cliques.empty() && curr.size() > max_cliques.back().size())
            max_cliques.clear();
        if (max_cliques.empty() || curr.size() == max_cliques.back().size())
            max_cliques.push_back(curr);
        return;
    }

    nanobot pivot = candidates.empty() ? *excluded.begin() : *candidates.begin();
    for (auto it = candidates.begin();
         it != candidates.end()
         && (max_cliques.empty()
             || curr.size() + candidates.size() >= max_cliques.back().size());) {
        const nanobot& v = *it;
        if (!g.has_edge(pivot, v)) {
            std::unordered_set<nanobot> new_curr(curr), new_cand(candidates), new_ex(excluded);
            new_curr.insert(v);
            std::erase_if(new_cand, [&](const nanobot& u) { return !g.has_edge(v, u); });
            std::erase_if(new_ex, [&](const nanobot& u) { return !g.has_edge(v, u); });
            Bron_Kerbosch(max_cliques, g, new_curr, new_cand, new_ex);

            excluded.insert(v);
            it = candidates.erase(it);
        } else {
            ++it;
        }
    }
}

int day_23_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

#if DAY_23 == 1
    graph::unweighted_graph<nanobot, true> in_range;
#elif DAY_23 == 2
    graph::unweighted_graph<nanobot, false> in_range;
#endif

    for (std::string line; std::getline(reader, line);) {
        std::istringstream parser(line);
        parser.ignore(5, '<');

        int x, y, z, r;
        parser >> x;
        parser.ignore(1, ',');
        parser >> y;
        parser.ignore(1, ',');
        parser >> z;
        parser.ignore(6, '=');
        parser >> r;

        nanobot curr(x, y, z, r);
        in_range.add_vertex(curr);

        for (const auto& v : in_range.vertices()) {
            if (curr != v) {
                const auto& [x1, y1, z1, r1] = v;
                auto p = {x, y, z};
                auto p1 = {x1, y1, z1};
#if DAY_23 == 1
                // Add edge iff the bots fall within range of each other
                std::size_t dist = std::transform_reduce(
                  p.begin(), p.end(), p1.begin(), std::size_t(0), std::plus<>(),
                  [](int a, int b) { return std::abs(a - b); });

                if (dist <= r)
                    in_range.force_add(curr, v);
                if (dist <= r1)
                    in_range.force_add(v, curr);
#elif DAY_23 == 2
                // Add if overlap -> distance between centers <= sum of ranges
                if (std::transform_reduce(p.begin(), p.end(), p1.begin(), std::size_t(0),
                                          std::plus<>(),
                                          [&](int a, int b) { return std::abs(a - b); })
                    <= r + r1)
                    in_range.force_add(curr, v);
#endif
            }
        }
    }

    auto vertices = in_range.vertices();

#if DAY_23 == 1
    auto strongest =
      *std::max_element(vertices.begin(), vertices.end(), [](const auto& x, const auto& y) {
          return std::get<3>(x) < std::get<3>(y);
      });

    std::cout << in_range.degree(strongest) + 1 << std::endl;
#elif DAY_23 == 2
    std::list<std::unordered_set<nanobot>> max_cliques;
    std::unordered_set<nanobot> init_curr, init_ex, init_cand(vertices.begin(), vertices.end());

    // Find the maximum clique
    Bron_Kerbosch(max_cliques, in_range, init_curr, init_cand, init_ex);

    /*
    auto max_c = max_cliques.front();

    // Convert to Integer optimization problem
    auto* solver = operations_research::MPSolver::CreateSolver("GLOP");
    const double inf = solver->infinity();
    auto* const sx = solver->MakeIntVar(0, inf, "x");
    auto* const sy = solver->MakeIntVar(0, inf, "y");
    auto* const sz = solver->MakeIntVar(0, inf, "z");

    // Add constraints on distance from each nanobot in clique
    for (const nanobot& n : max_c) {
        auto [x, y, z, r] = n;
        for (std::size_t i = 0; i < 8; ++i) {
            int wx = (i % 2) ? 1 : -1;
            int wy = (i / 2) % 2 ? 1 : -1;
            int wz = i / 4 ? 1 : -1;
            int sum = x * wx + y * wy + z * wz;
            auto* const c = solver->MakeRowConstraint(-inf, r + sum);
            c->SetCoefficient(sx, wx);
            c->SetCoefficient(sy, wy);
            c->SetCoefficient(sz, wz);
        }
    }

    // Minimize distance to origin
    auto* const obj = solver->MutableObjective();
    obj->SetCoefficient(sx, 1);
    obj->SetCoefficient(sy, 1);
    obj->SetCoefficient(sz, 1);
    obj->SetMinimization();

    solver->Solve();
    std::cout << int(obj->Value()) << std::endl;
    */

    // Alternate way of solving: Exploit knowns:
    // 1. we know region of intersection is entirely in (+x, +y, +z) octant (through extensive
    // debugging)
    // 2. The minimum distance must lie on a particular surface (each coord < center)
    // 3. Manhattan distance-wise, this plane forms part of a "sphere" centered on the origin
    // 4. This surface is contained in part in all other octahedra, which must have similar surfaces
    // closer to the origin
    int dist = -1;
    for (const auto& max_c : max_cliques) {
        int curr_dist = 0;
        for (const nanobot& n : max_c) {
            auto [x, y, z, r] = n;
            curr_dist = std::max(dist, x + y + z - r);
        }
        dist = dist == -1 ? curr_dist : std::min(curr_dist, dist);
    }
    std::cout << dist << std::endl;
#endif

    return 0;
}

#endif // DAY_23
