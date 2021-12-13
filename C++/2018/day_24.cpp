#ifndef DAY_24
#define DAY_24

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <tuple>
#include <unordered_set>
#include <vector>

struct group {
    std::size_t units;
    std::size_t hp;
    std::size_t atk;
    std::string elem;
    std::size_t spd;
    std::unordered_set<std::string> weak;
    std::unordered_set<std::string> immune;
};

// Run a simulation with a boost
// Return {immune survives, count}
std::pair<bool, std::size_t> run_simulation(std::vector<group> immune_system,
                                            std::vector<group> infection, std::size_t boost) {
    for (group& g : immune_system)
        g.atk += boost;

    while (!immune_system.empty() && !infection.empty()) {
        std::vector<std::pair<std::size_t, bool>> unit_indices;
        for (std::size_t i = 0; i < immune_system.size(); ++i)
            unit_indices.emplace_back(i, false);
        for (std::size_t i = 0; i < infection.size(); ++i)
            unit_indices.emplace_back(i, true);

        std::vector<std::pair<std::size_t, bool>> targets(unit_indices);
        std::vector<bool> targeted(unit_indices.size(), false);

        // Determine targeting order
        std::sort(
          unit_indices.begin(), unit_indices.end(),
          [&](const std::pair<std::size_t, bool>& p1, const std::pair<std::size_t, bool>& p2) {
              const std::vector<group>& v1 = p1.second ? infection : immune_system;
              const std::vector<group>& v2 = p2.second ? infection : immune_system;

              return std::make_pair(v1[p1.first].atk * v1[p1.first].units, v1[p1.first].spd)
                   > std::make_pair(v2[p2.first].atk * v2[p2.first].units, v2[p2.first].spd);
          });

        // Determine target
        for (const auto& p : unit_indices) {
            const std::vector<group>& allies = p.second ? infection : immune_system;
            const std::vector<group>& opponents = p.second ? immune_system : infection;

            auto it = std::max_element(
              opponents.begin(), opponents.end(), [&](const group& g1, const group& g2) {
                  std::size_t effect_1 = allies[p.first].atk * allies[p.first].units,
                              effect_2(effect_1);
                  std::size_t pos1(&g1 - opponents.data()), pos2(&g2 - opponents.data());
                  if (targeted[pos1 + !p.second * immune_system.size()])
                      return true;
                  if (targeted[pos2 + !p.second * immune_system.size()])
                      return false;
                  if (g1.immune.contains(allies[p.first].elem))
                      effect_1 = 0;
                  else if (g1.weak.contains(allies[p.first].elem))
                      effect_1 *= 2;
                  if (g2.immune.contains(allies[p.first].elem))
                      effect_2 = 0;
                  else if (g2.weak.contains(allies[p.first].elem))
                      effect_2 *= 2;

                  return std::make_tuple(effect_1, g1.atk * g1.units, g1.spd)
                       < std::make_tuple(effect_2, g2.atk * g2.units, g2.spd);
              });
            if (!targeted[(it - opponents.begin()) + !p.second * immune_system.size()]) {
                targets[p.first + p.second * immune_system.size()] = {it - opponents.begin(),
                                                                      !p.second};
                if (!it->immune.contains(allies[p.first].elem))
                    targeted[(it - opponents.begin()) + !p.second * immune_system.size()] = true;
            } else {
                targets[p.first + p.second * immune_system.size()].second = p.second;
            }
        }

        // Determine attack order
        std::sort(
          unit_indices.begin(), unit_indices.end(),
          [&](const std::pair<std::size_t, bool>& p1, const std::pair<std::size_t, bool>& p2) {
              const std::vector<group>& v1 = p1.second ? infection : immune_system;
              const std::vector<group>& v2 = p2.second ? infection : immune_system;

              return v1[p1.first].spd > v2[p2.first].spd;
          });

        // Attack
        for (const auto& p : unit_indices) {
            const auto& t = targets[p.first + p.second * immune_system.size()];
            if (p.second == t.second)
                continue;

            std::vector<group>& allies = p.second ? infection : immune_system;
            std::vector<group>& opponents = p.second ? immune_system : infection;

            if (allies[p.first].units == 0
                || opponents[t.first].immune.contains(allies[p.first].elem))
                continue;

            std::size_t damage = allies[p.first].units * allies[p.first].atk;
            if (opponents[t.first].weak.contains(allies[p.first].elem))
                damage *= 2;

            std::size_t destroyed = damage / opponents[t.first].hp;
            if (opponents[t.first].units < destroyed)
                opponents[t.first].units = 0;
            else
                opponents[t.first].units -= destroyed;
        }

        std::erase_if(immune_system, [](const group& g) { return g.units == 0; });
        std::erase_if(infection, [](const group& g) { return g.units == 0; });
    }

    return {infection.empty(),
            std::transform_reduce(immune_system.begin(), immune_system.end(), std::size_t(0),
                                  std::plus<>(), std::mem_fn(&group::units))
              + std::transform_reduce(infection.begin(), infection.end(), std::size_t(0),
                                      std::plus<>(), std::mem_fn(&group::units))};
}

int day_24_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::string line;

    std::vector<group> immune_system, infection;

    bool processing_immune = true;
    std::getline(reader, line);
    while (std::getline(reader, line)) {
        if (line.empty())
            continue;

        if (line.front() == 'I') {
            processing_immune = false;
            continue;
        }

        std::vector<group>& faction = processing_immune ? immune_system : infection;

        std::istringstream parser(line);
        std::string word;
        bool has_additional = line.find('(') != std::string::npos;
        group g;
        parser >> word;
        g.units = std::stoul(word);
        for (auto i = 0; i < 4; ++i)
            parser >> word;
        g.hp = std::stoul(word);

        for (auto i = 0; i < 2; ++i)
            parser >> word;

        while (has_additional) {
            parser >> word;
            if (word.front() == '(')
                word = word.substr(1);
            std::unordered_set<std::string>& ins = word == "immune" ? g.immune : g.weak;
            parser >> word;
            bool found_all = false;
            while (!found_all) {
                parser >> word;
                if (word.back() == ';') {
                    found_all = true;
                } else if (word.back() == ')') {
                    found_all = true;
                    has_additional = false;
                }
                word.pop_back();
                ins.insert(word);
            }
        }

        for (auto i = 0; i < 6; ++i)
            parser >> word;
        g.atk = std::stoul(word);
        parser >> g.elem;

        for (auto i = 0; i < 4; ++i)
            parser >> word;
        g.spd = std::stoul(word);

        faction.push_back(std::move(g));
    }

    std::size_t lower = 0;
    std::size_t upper = 1;
    while (!run_simulation(immune_system, infection, upper).first)
        upper *= 2;

    std::size_t result = run_simulation(immune_system, infection, upper).second;
    while (upper > lower + 2) {
        std::size_t mid = (lower + upper) / 2;
        auto r = run_simulation(immune_system, infection, mid);
        if (r.first) {
            result = r.second;
            upper = mid;
        } else {
            lower = mid;
        }
    }

    std::cout << result << std::endl;

    // There's a possibility the simulation deadlocks the two sides, so when the range is down to 2,
    // handle separately
    auto f = run_simulation(immune_system, infection, upper - 1);
    if (f.first)
        std::cout << f.second << std::endl;
    else
        std::cout << "Finished" << std::endl;

    return 0;
}

#endif // DAY_24
