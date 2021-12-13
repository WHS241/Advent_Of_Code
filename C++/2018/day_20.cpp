#ifndef DAY_20
#define DAY_20

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

#include <util/pair_hash.h>

typedef std::pair<long, long> point;
typedef util::pair_hash<long, long> point_hash;
typedef std::unordered_map<point, std::unordered_set<point, point_hash>, point_hash> grid;

struct node {
    // one of these two must be empty
    std::string path;
    std::list<node*> children;

    bool is_and;

    ~node() {
        for (node* p : children)
            delete p;
    }

    // Populates the grid and returns the set of terminal points
    std::unordered_set<point, point_hash> populate(const point& start, grid& g) {
        if (!path.empty()) {
            point curr(start);
            for (char c : path) {
                point next(curr);
                switch (c) {
                case 'E':
                    ++next.first;
                    break;

                case 'W':
                    --next.first;
                    break;

                case 'N':
                    ++next.second;
                    break;

                case 'S':
                    --next.second;
                    break;
                }

                g[curr].insert(next);
                g[next].insert(curr);
                curr = next;
            }
            return {curr};
        }

        if (children.empty())
            return {start};

        if (is_and) {
            // Use the outputs from each child node as inputs for following child
            std::unordered_set<point, point_hash> curr_ends;
            curr_ends.insert(start);
            for (node* ptr : children) {
                std::unordered_set<point, point_hash> next;
                for (const point& p : curr_ends) {
                    for (const point& p2 : ptr->populate(p, g))
                        next.insert(p2);
                }
                curr_ends = std::move(next);
            }

            return curr_ends;
        }

        // OR node: populate based on each child separately
        std::unordered_set<point, point_hash> result;
        for (node* ptr : children)
            for (const point& p : ptr->populate(start, g))
                result.insert(p);

        return result;
    }
};

void build_tree(node* root, const std::string& regex, std::size_t& i) {
    while (i < regex.size()) {
        switch (regex[i++]) {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            if (root->is_and) {
                // If last child is leaf, append. Else create a new last child
                if (root->children.empty() || !root->children.back()->children.empty())
                    root->children.push_back(new node());
                root->children.back()->path.push_back(regex[i - 1]);
            } else {
                // By design, an OR node can only have leafs and AND nodes as children
                // Treat the last child as the "root" for the current letter
                if (root->children.back()->children.empty()
                    || !root->children.back()->children.back()->children.empty()) {
                    node* inter = new node();
                    inter->is_and = true;
                    inter->children.push_back(root->children.back());
                    root->children.back() = inter;
                    inter->children.push_back(new node());
                }
                root->children.back()->children.back()->path.push_back(regex[i - 1]);
            }
            break;

        case ')':
            return;

        case '|':
            if (root->is_and && root->children.size() > 1) {
                // Preserve AND relation in a new child node
                node* inter = new node();
                inter->children = root->children;
                inter->is_and = true;
                root->children.clear();
                root->children.push_back(inter);
            } else if (root->children.empty())
                root->children.push_back(new node());
            root->is_and = false;
            root->children.push_back(new node());
            break;

        case '(':
            {
                // Generate a subtree for expression in parentheses
                // This subtree needs to be ANDed with the last child
                node* new_root;
                if (root->is_and) {
                    root->children.push_back(new node());
                    root->children.back()->is_and = true;
                    new_root = root->children.back();
                } else {
                    node* inter = new node();
                    inter->is_and = true;
                    inter->children.push_back(root->children.back());
                    root->children.back() = inter;
                    inter->children.push_back(new node());
                    inter->children.back()->is_and = true;
                    new_root = inter->children.back();
                }
                build_tree(new_root, regex, i);
            }
            break;
        }
    }
}

int day_20_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::unique_ptr<node> root(new node());
    root->is_and = true;

    std::string regex;
    std::getline(reader, regex);
    regex = regex.substr(1, regex.size() - 2);
    std::size_t i = 0;

    build_tree(root.get(), regex, i);

    grid layout;
    root->populate({0, 0}, layout);

    std::unordered_map<point, std::size_t, point_hash> distance;
    distance[{0, 0}] = 0;

    std::list<point> frontier;
    frontier.emplace_back(0, 0);

    while (!frontier.empty()) {
        point curr = frontier.front();
        frontier.pop_front();

        for (const point& next : layout[curr]) {
            if (!distance.contains(next)) {
                distance.emplace(next, distance[curr] + 1);
                frontier.push_back(next);
            }
        }
    }

    std::cout << std::max_element(distance.begin(), distance.end(),
                                  [](const auto& x, const auto& y) { return x.second < y.second; })
                   ->second
              << std::endl;
    std::cout << std::count_if(distance.begin(), distance.end(), [](const auto& x) {
        return x.second >= 1000;
    }) << std::endl;

    return 0;
}

#endif // DAY_20
