#ifndef DAY_21
#define DAY_21 2

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include <structures/graph.h>

#include <graph/bipartite.h>

int day_21_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::vector<std::string>> ingredients_list;
    std::map<std::string, std::set<std::string>> candidates;

    for (std::string line; std::getline(reader, line);) {
        std::size_t pos = line.find('(');
        std::string ingredients = line.substr(0, pos);
        std::string translation = line.substr(pos + 1);
        translation.pop_back();
        std::istringstream parser(ingredients);
        std::vector<std::string> parsed(std::istream_iterator<std::string>{parser},
                                        std::istream_iterator<std::string>{});
        std::sort(parsed.begin(), parsed.end());

        parser = std::istringstream(translation);
        std::vector<std::string> parsed_trans;
        for (std::string word; parser >> word;) {
            if (word == "contains")
                continue;
            if (word.back() == ',')
                word.pop_back();
            parsed_trans.push_back(word);
        }

        for (const std::string& s : parsed_trans) {
            if (!candidates.contains(s)) {
                candidates[s] = std::set<std::string>(parsed.begin(), parsed.end());
            } else {
                std::vector<std::string> prev(candidates[s].begin(), candidates[s].end());
                std::vector<std::string> intersect;
                std::set_intersection(parsed.begin(), parsed.end(), prev.begin(), prev.end(),
                                      std::back_inserter(intersect));
                candidates[s] = std::set<std::string>(intersect.begin(), intersect.end());
            }
        }

        ingredients_list.push_back(parsed);
    }

    // Part 1
    std::size_t count = 0;
    for (const auto& v : ingredients_list)
        for (const auto& s : v)
            if (std::none_of(candidates.begin(), candidates.end(),
                             [&](const std::pair<const std::string, std::set<std::string>>& p) {
                                 return p.second.contains(s);
                             })) {
                ++count;
            }

    std::cout << count << std::endl;

    // Part 2
    graph::unweighted_graph<std::string, false> possible;
    for (const auto& p : candidates) {
        if (!possible.has_vertex(p.first))
            possible.add_vertex(p.first);
        for (const std::string& s : p.second) {
            if (!possible.has_vertex(s))
                possible.add_vertex(s);
            possible.force_add(p.first, s);
        }
    }

    auto matches = graph_alg::maximum_bipartite_matching(possible);
    std::map<std::string, std::string> match_map;
    for (auto& p : matches) {
        if (candidates.contains(p.second))
            std::swap(p.first, p.second);
        match_map.insert(p);
    }

    std::transform(match_map.begin(), match_map.end(),
                   std::ostream_iterator<std::string>(std::cout, ","),
                   std::mem_fn(&std::pair<const std::string, std::string>::second));
    std::cout << std::endl;

    return 0;
}

#endif // DAY_21
