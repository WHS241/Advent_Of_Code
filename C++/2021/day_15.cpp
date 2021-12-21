#ifndef DAY_15
#define DAY_15

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <graph/path.h>
#include <structures/graph.h>
#include <util/pair_hash.h>

struct p_queue_node {
    std::pair<std::size_t, std::size_t> coords;
    std::size_t key;
    std::size_t heuristic;

    auto operator<=>(const p_queue_node& rhs) const {
        return key + heuristic <=> rhs.key + rhs.heuristic;
    }
};

int day_15_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<std::string> cavern(std::istream_iterator<std::string>{reader},
                                    std::istream_iterator<std::string>{});

    std::size_t num_copies = std::stoul(argv[2]);

    heap::Fibonacci<p_queue_node> frontier;
    std::unordered_map<std::pair<std::size_t, std::size_t>,
                       heap::node_base<p_queue_node>::node_wrapper,
                       util::pair_hash<std::size_t, std::size_t>>
      in_frontier;

    frontier.add(p_queue_node{std::make_pair(0UL, 0UL), 0UL,
                              num_copies * (cavern.size() + cavern.front().size()) - 2});
    std::pair<std::size_t, std::size_t> target =
      std::make_pair(num_copies * cavern.size() - 1, num_copies * cavern.front().size() - 1);
    std::size_t result = 0;
    std::unordered_set<std::pair<std::size_t, std::size_t>,
                       util::pair_hash<std::size_t, std::size_t>>
      visited;

    while (result == 0) {
        p_queue_node curr = frontier.remove_root();
        in_frontier.erase(curr.coords);
        visited.insert(curr.coords);
        if (curr.coords == target)
            result = curr.key;

        std::array<std::pair<std::size_t, std::size_t>, 4> neighbors = {
          std::make_pair(curr.coords.first + 1, curr.coords.second),
          std::make_pair(curr.coords.first - 1, curr.coords.second),
          std::make_pair(curr.coords.first, curr.coords.second - 1),
          std::make_pair(curr.coords.first, curr.coords.second + 1)};

        for (auto& p : neighbors) {
            if (!visited.contains(p) && p.first < num_copies * cavern.size()
                && p.second < num_copies * cavern.front().size()) {
                std::size_t risk_level =
                  (cavern[p.first % cavern.size()][p.second % cavern.front().size()] - '1'
                   + (p.first / cavern.size()) + (p.second / cavern.front().size()))
                    % 9
                  + 1;
                std::size_t heuristic =
                  num_copies * (cavern.size() + cavern.front().size()) - (2 + p.first + p.second);
                if (in_frontier.contains(p)) {
                    if (curr.key + risk_level < (*in_frontier[p].get())->key)
                        frontier.decrease(in_frontier[p], {p, curr.key + risk_level, heuristic});
                } else {
                    in_frontier[p] =
                      frontier.add(p_queue_node{p, curr.key + risk_level, heuristic});
                }
            }
        }
    }

    std::cout << result << std::endl;

    return 0;
}

#endif // DAY_15
