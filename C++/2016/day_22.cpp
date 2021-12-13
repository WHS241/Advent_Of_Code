#ifndef DAY_22
#define DAY_22 2

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <util/pair_hash.h>

int day_22_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

#if DAY_22 == 1
    std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> sizes;
    std::map<std::size_t, std::unordered_set<std::string>> on_used;
    std::map<std::size_t, std::unordered_set<std::string>> on_avail;

    std::size_t i = 0;
    for (std::string line; std::getline(reader, line);) {
        if (i < 2) {
            ++i;
            continue;
        }

        std::istringstream line_read(line);
        std::string node;
        line_read >> node;
        node = node.substr(10);
        std::string word;
        std::size_t used, avail;
        line_read >> word >> word;
        used = std::stoul(word.substr(0, word.size() - 1));
        line_read >> word;
        avail = std::stoul(word.substr(0, word.size() - 1));
        sizes[node] = std::make_pair(used, avail);
        on_used[used].insert(node);
        on_avail[avail].insert(node);
    }

    std::size_t viable_pairs = 0;
    for (const std::pair<const std::size_t, std::unordered_set<std::string>>& u : on_used) {
        if (!u.first)
            continue;
        for (auto it = on_avail.lower_bound(u.first); it != on_avail.end(); ++it) {
            std::size_t num_pairs = it->second.size() * u.second.size();
            for (const std::string& n : u.second)
                if (it->second.count(n))
                    --num_pairs;
            viable_pairs += num_pairs;
        }
    }

    std::cout << viable_pairs << std::endl;
#elif DAY_22 == 2
    std::vector<std::vector<bool>> is_wall;
    std::pair<std::size_t, std::size_t> hole_pos;
    std::size_t i = 0;
    for (std::string line; std::getline(reader, line);) {
        if (i < 2) {
            ++i;
            continue;
        }
        std::istringstream line_read(line);
        std::string word;
        line_read >> word;
        if (word.substr(word.size() - 2) == "y0")
            is_wall.emplace_back();

        std::size_t x, y, pos1, pos2;
        pos1 = word.find('x');
        pos2 = word.find('y', pos1);
        x = std::stoul(word.substr(pos1 + 1, pos2 - pos1 - 2));
        y = std::stoul(word.substr(pos2 + 1));
        std::size_t used;
        line_read >> word >> word;
        used = std::stoul(word.substr(0, word.size() - 1));
        is_wall.back().push_back(used > 200);

        if (used == 0)
            hole_pos = std::make_pair(x, y);
    }

    for (std::size_t i = 0; i < is_wall.size(); ++i) {
        for (std::size_t j = 0; j < is_wall[i].size(); ++j) {
            if (i == 0 && j == 0)
                std::cout << 'E';
            else if (i == is_wall.size() - 1 && j == 0)
                std::cout << 'G';
            else if (is_wall[i][j])
                std::cout << '#';
            else if (std::make_pair(i, j) == hole_pos)
                std::cout << 'O';
            else
                std::cout << '.';
        }
        std::cout << std::endl;
    }
#endif

    return 0;
}

#endif // DAY_22
