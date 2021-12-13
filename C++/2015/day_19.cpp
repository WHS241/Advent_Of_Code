#ifndef DAY_19
#define DAY_19 2

#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "structures/heap"

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::unordered_map<std::string, std::list<std::string>> reactions;
    std::unordered_set<std::string> results;

    std::ifstream reader(argv[1]);

    for (std::string line; std::getline(reader, line);) {
        if (line.empty())
            break;

        std::istringstream line_read(line);
        std::string start, end;

        line_read >> start >> end >> end;
        reactions[start].push_back(end);
    }

    std::string start;
    reader >> start;

    // Part 1
    std::string atom;
    for (std::size_t i = 0; i < start.size(); ++i) {
        if (std::isupper(start[i]))
            atom.clear();
        atom.push_back(start[i]);
        if (reactions.count(atom))
            for (const std::string& s : reactions[atom])
                results.insert(start.substr(0, i + 1 - atom.size()) + s + start.substr(i + 1));
    }
    std::cout << results.size() << std::endl;

    // Part 2: A* search from final molecule down to electron
    std::size_t max_product_size = 0;
    std::unordered_map<std::string, std::string> reversed_reactions;
    for (const std::pair<const std::string, std::list<std::string>>& p : reactions) {
        for (const std::string& r : p.second) {
            if (!reversed_reactions.emplace(r, p.first).second)
                throw std::runtime_error("Duplicate");
            max_product_size = std::max(max_product_size, r.size());
        }
    }

    std::string target = "e";

    std::unordered_map<std::string, std::size_t> distance;
    distance[start] = 0;

    auto compare = [&target, &max_product_size](const std::pair<std::string, std::size_t>& x,
                                                const std::pair<std::string, std::size_t>& y) {
        return (x.second + x.first.size() - target.size()) / max_product_size
             < (y.second + y.first.size() - target.size()) / max_product_size;
    };

    heap::Fibonacci<std::pair<std::string, std::size_t>, decltype(compare)> frontier(compare);
    frontier.insert(std::make_pair(start, 0));

    while (!frontier.empty()) {
        auto [current, dist] = frontier.remove_root();

        std::list<std::string> potential_products;
        for (std::size_t i = 0; i < current.size(); ++i) {
            char c = current[i];

            for (std::string& s : potential_products)
                s.push_back(c);

            if (std::isupper(c)) {
                potential_products.emplace_back();
                potential_products.back().push_back(c);
            }

            for (const std::string& s : potential_products)
                if (reversed_reactions.count(s)) {
                    std::string reactant = current.substr(0, i + 1 - s.size())
                                         + reversed_reactions[s] + current.substr(i + 1);
                    if (!distance.count(reactant) || distance.at(reactant) > dist + 1) {
                        distance[reactant] = distance[current] + 1;
                        frontier.insert(std::make_pair(reactant, dist + 1));
                    }
                }

            if (potential_products.front().size() == max_product_size)
                potential_products.pop_front();
        }
        if (distance.count(target))
            break;
    }

    std::cout << distance[target] << std::endl;

    return 0;
}

#endif // DAY_19
