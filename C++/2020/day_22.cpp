#ifndef DAY_22
#define DAY_22 2

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <unordered_set>

#include <util/pair_hash.h>

namespace std {
template<> struct hash<std::pair<std::list<std::size_t>, std::list<std::size_t>>> {
    std::hash<std::size_t> hasher;

    std::size_t
      operator()(const std::pair<std::list<std::size_t>, std::list<std::size_t>>& v) const {
        std::size_t h1 = std::accumulate(
          v.first.begin(), v.first.end(), std::size_t(0),
          [&](std::size_t acc, std::size_t x) { return util::asym_combine_hash(acc, hasher(x)); });
        std::size_t h2 = std::accumulate(
          v.second.begin(), v.second.end(), std::size_t(0),
          [&](std::size_t acc, std::size_t x) { return util::asym_combine_hash(acc, hasher(x)); });
        return util::asym_combine_hash(h1, h2);
    }
};
} // namespace std

bool recurse_combat(std::pair<std::list<std::size_t>, std::list<std::size_t>>& decks) {
    std::unordered_set<std::pair<std::list<std::size_t>, std::list<std::size_t>>> vals;
    while (!decks.first.empty() && !decks.second.empty()) {
        if (vals.contains(decks))
            return true;

        vals.insert(decks);
        std::size_t v1 = decks.first.front(), v2 = decks.second.front();
        decks.first.pop_front();
        decks.second.pop_front();
        bool first_wins;
        if (decks.first.size() >= v1 && decks.second.size() >= v2) {
            auto next(decks);
            next.first.resize(v1);
            next.second.resize(v2);
            first_wins = recurse_combat(next);
        } else {
            first_wins = v1 > v2;
        }

        if (first_wins) {
            decks.first.push_back(v1);
            decks.first.push_back(v2);
        } else {
            decks.second.push_back(v2);
            decks.second.push_back(v1);
        }
    }

    return decks.second.empty();
}

int day_22_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::pair<std::list<std::size_t>, std::list<std::size_t>> decks;
    bool process1 = true;
    std::string line;
    std::getline(reader, line);

    while (std::getline(reader, line)) {
        if (line.empty())
            continue;
        if (std::isalpha(line.front())) {
            process1 = false;
            continue;
        }
        std::list<std::size_t>& curr = process1 ? decks.first : decks.second;
        curr.push_back(std::stoul(line));
    }

#if DAY_22 == 1
    while (!decks.first.empty() && !decks.second.empty()) {
        if (decks.first.front() < decks.second.front()) {
            decks.second.splice(decks.second.end(), decks.second, decks.second.begin());
            decks.second.splice(decks.second.end(), decks.first, decks.first.begin());
        } else {
            decks.first.splice(decks.first.end(), decks.first, decks.first.begin());
            decks.first.splice(decks.first.end(), decks.second, decks.second.begin());
        }
    }
#elif DAY_22 == 2
    recurse_combat(decks);
#endif

    std::size_t score = 0, i = 1;
    std::list<std::size_t>& winner = decks.first.empty() ? decks.second : decks.first;

    for (auto it = winner.rbegin(); it != winner.rend(); ++it)
        score += *it * i++;

    std::cout << score << std::endl;

    return 0;
}

#endif // DAY_22
