#ifndef DAY_7
#define DAY_7 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int day_7_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::unordered_map<std::string, std::size_t> weights;
    std::unordered_map<std::string, std::list<std::string>> children;

    std::unordered_set<std::string> roots;
#if DAY_7 == 2
    std::unordered_map<std::string, std::string> parent;
#endif

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string id;
        line_read >> id;
        roots.insert(id);

        std::string word;
        line_read >> word;
        weights[id] = std::stoul(word.substr(1, word.size() - 2));

        line_read >> word;

        for (std::string child; line_read >> child;) {
            if (child.back() == ',')
                child = child.substr(0, child.size() - 1);
#if DAY_7 == 2
            parent.emplace(child, id);
#endif
            children[id].push_back(std::move(child));
        }
    }

    for (const std::pair<const std::string, std::list<std::string>>& p : children)
        for (const std::string& c : p.second)
            roots.erase(c);

#if DAY_7 == 1
    std::cout << *roots.begin() << std::endl;
#elif DAY_7 == 2
    std::cout << children[*roots.begin()].size() << std::endl;
    std::unordered_map<std::string, std::size_t> num_children;
    std::vector<std::string> to_calc;
    for (const std::pair<const std::string, std::list<std::string>>& p : children) {
        num_children[p.first] = p.second.size();
        for (const std::string& s : p.second)
            if (!children.count(s))
                to_calc.push_back(s);
    }

    std::unordered_map<std::string, long> stack_weight;
    while (!to_calc.empty()) {
        std::string curr = to_calc.back();
        to_calc.pop_back();
        stack_weight[curr] = weights[curr];
        for (const std::string& c : children[curr])
            stack_weight[curr] += stack_weight[c];
        if (parent.count(curr)) {
            --num_children[parent[curr]];
            if (!num_children[parent[curr]])
                to_calc.push_back(parent[curr]);
        }
    }

    std::string curr = *roots.begin();
    long offset;
    while (true) {
        auto it =
          std::find_if_not(children[curr].begin(), children[curr].end(),
                           [&children, &stack_weight, &curr](const std::string& s) {
                               return stack_weight[s] == stack_weight[children[curr].front()];
                           });
        if (it == children[curr].end()) {
            std::cout << weights[curr] - offset << std::endl;
            break;
        }

        if (children[curr].size() == 2) {
            if (!((offset < 0) ^
                  (stack_weight[children[curr].front()] < stack_weight[children[curr].back()])))
                --it;
        } else if (*it != children[curr].back() &&
                   stack_weight[*it] == stack_weight[children[curr].back()])
            --it;

        offset =
          stack_weight[*it] - stack_weight[(it == children[curr].begin() ? children[curr].back()
                                                                         : children[curr].front())];
        curr = *it;
    }
#endif

    return 0;
}

#endif // DAY_7
