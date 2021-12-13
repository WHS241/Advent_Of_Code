#ifndef DAY_15
#define DAY_15 2

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <tuple>
#include <vector>

constexpr char EMPTY('.'), ELF('E');
constexpr std::size_t GOBLIN_ATK = 3;
constexpr std::size_t HP = 200;

std::array<std::pair<std::size_t, std::size_t>, 4>
  get_neighbors(const std::pair<std::size_t, std::size_t>& p) {
    return {std::make_pair(p.first - 1, p.second), std::make_pair(p.first + 1, p.second),
            std::make_pair(p.first, p.second - 1), std::make_pair(p.first, p.second + 1)};
}

// [ meet_condition, rounds, HP ]
std::tuple<bool, std::size_t, std::size_t> get_outcome(std::vector<std::string> cave,
                                                       std::size_t elf_atk, bool no_deaths) {
    std::map<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>>
      units; // units[pos] = {hp, atk}
    for (std::size_t i = 0; i < cave.size(); ++i)
        for (std::size_t j = 0; j < cave[0].size(); ++j)
            if (std::isalpha(cave[i][j]))
                units.emplace(std::make_pair(i, j),
                              std::make_pair(HP, cave[i][j] == ELF ? elf_atk : GOBLIN_ATK));

    auto enemies = [&cave](const std::pair<std::size_t, std::size_t>& p1,
                           const std::pair<std::size_t, std::size_t>& p2) {
        return std::isalpha(cave[p1.first][p1.second]) && std::isalpha(cave[p2.first][p2.second])
            && cave[p1.first][p1.second] != cave[p2.first][p2.second];
    };

    bool finished = false;
    std::size_t num_completed = 0;
    while (!finished) {
        auto copy(units);

        for (std::pair<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>> v :
             units) {
            finished = true;
            std::pair<std::size_t, std::size_t> p = v.first;
            std::array<std::pair<std::size_t, std::size_t>, 4> neighbors = get_neighbors(p);
            bool in_range = std::any_of(neighbors.begin(), neighbors.end(),
                                        std::bind(enemies, std::cref(p), std::placeholders::_1));

            if (!in_range) {
                // move

                // In range of enemies
                std::set<std::pair<std::size_t, std::size_t>> targets;
                for (auto v2 : copy) {
                    auto& p2 = v2.first;
                    if (p != p2 && cave[p.first][p.second] != cave[p2.first][p2.second]) {
                        finished = false;
                        for (auto p3 : get_neighbors(p2))
                            if (cave[p3.first][p3.second] == EMPTY)
                                targets.insert(p3);
                    }
                }

                // BFS from all targets simultaneously
                std::list<std::pair<std::pair<std::size_t, std::size_t>, std::size_t>> frontier;
                std::transform(targets.begin(), targets.end(), std::back_inserter(frontier),
                               [](auto p) { return std::make_pair(p, 0UL); });

                std::map<std::pair<std::size_t, std::size_t>, std::size_t> distance(
                  frontier.begin(), frontier.end());
                while (!frontier.empty()
                       && !std::all_of(neighbors.begin(), neighbors.end(),
                                       [&distance](auto p) { return distance.count(p); })) {
                    auto curr = frontier.front();
                    frontier.pop_front();

                    for (auto next : get_neighbors(curr.first)) {
                        if (cave[next.first][next.second] == EMPTY && !distance.count(next)) {
                            frontier.emplace_back(next, curr.second + 1);
                            distance.insert(frontier.back());
                        }
                    }
                }

                auto move_to = *std::min_element(
                  neighbors.begin(), neighbors.end(), [&distance](auto p, auto p2) {
                      return distance.count(p)
                          && (!distance.count(p2)
                              || std::tie(distance[p], p) < std::tie(distance[p2], p2));
                  });

                if (distance.count(move_to)) {
                    cave[move_to.first][move_to.second] = cave[p.first][p.second];
                    cave[p.first][p.second] = EMPTY;
                    copy.erase(p);

                    p = move_to;
                    copy.emplace(p, v.second);
                    neighbors = get_neighbors(p);
                    in_range = std::any_of(neighbors.begin(), neighbors.end(),
                                           std::bind(enemies, std::cref(p), std::placeholders::_1));
                }
            }

            // attack
            if (in_range) {
                finished = false;
                auto target = *std::min_element(
                  neighbors.begin(), neighbors.end(),
                  [&copy, &enemies, &p](const std::pair<std::size_t, std::size_t>& p1,
                                        const std::pair<std::size_t, std::size_t>& p2) {
                      return enemies(p, p1)
                          && (!enemies(p, p2)
                              || std::tie(copy[p1].first, p1) < std::tie(copy[p2].first, p2));
                  });
                if (copy[target].first < copy[p].second) {
                    if (no_deaths && cave[target.first][target.second] == ELF)
                        return std::make_tuple(false, 0, 0);
                    copy.erase(target);
                    if (p < target)
                        units.erase(target);
                    cave[target.first][target.second] = EMPTY;
                } else {
                    copy[target].first -= copy[p].second;
                }
            }

            if (finished) {
                break;
            }
        }

        if (!finished)
            ++num_completed;
        units = std::move(copy);
    }

    // outcome
    std::size_t hp_left =
      std::transform_reduce(units.begin(), units.end(), std::size_t(0), std::plus<>(),
                            [](auto p) { return p.second.first; });
    return std::make_tuple(cave[units.begin()->first.first][units.begin()->first.second] == ELF,
                           num_completed, hp_left);
}

int day_15_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> cave;
    for (std::string line; std::getline(reader, line);)
        cave.push_back(line);

#if DAY_15 == 1
    auto [success, num_completed, hp_left] = get_outcome(cave, 3, false);
    std::cout << (success ? "Elves" : "Goblins") << " win" << std::endl;
    std::cout << num_completed << '*' << hp_left << '=' << num_completed * hp_left << std::endl;
#elif DAY_15 == 2
    std::size_t min(GOBLIN_ATK), max(HP);
    std::size_t num_rounds, total_hp;
    while (max - min > 1) {
        std::size_t curr = (max + min) / 2;
        auto [success, r, hp] = get_outcome(cave, curr, true);
        if (success) {
            max = curr;
            num_rounds = r;
            total_hp = hp;
        } else {
            min = curr;
        }
    }

    std::cout << "Min ATK: " << max << std::endl;
    std::cout << num_rounds << '*' << total_hp << '=' << num_rounds * total_hp << std::endl;
#endif

    return 0;
}

#endif // DAY_15
