#ifndef DAY_22
#define DAY_22 2

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <utility>
#include <vector>

constexpr std::size_t BASE = 20183;
constexpr std::size_t X_MULT = 16807;
constexpr std::size_t Y_MULT = 48271 % BASE;

struct state {
    std::size_t choice;
    std::size_t x;
    std::size_t y;

    auto operator<=>(const state&) const = default;
};

int day_22_main(int argc, char** argv) {
    if (argc < 4)
        return 1;

    std::size_t depth = std::stoul(argv[1]), px = std::stoul(argv[2]), py = std::stoul(argv[3]);

    std::vector<std::vector<std::size_t>> erosion(px + 1, std::vector<std::size_t>(py + 1));
    for (std::size_t i = 0; i <= px; ++i) {
        for (std::size_t j = 0; j <= py; ++j) {
            if ((i == 0 && j == 0) || (i == px && j == py))
                erosion[i][j] = depth % BASE;
            else if (i == 0)
                erosion[i][j] = (Y_MULT * j + depth) % BASE;
            else if (j == 0)
                erosion[i][j] = (X_MULT * i + depth) % BASE;
            else
                erosion[i][j] = (erosion[i - 1][j] * erosion[i][j - 1] + depth) % BASE;
        }
    }

#if DAY_22 == 1
    std::size_t total = 0;
    for (std::size_t i = 0; i <= px; ++i)
        for (std::size_t j = 0; j <= py; ++j)
            total += erosion[i][j] % 3;

    std::cout << total << std::endl;
#elif DAY_22 == 2
    state terminal{1, px, py};
    state initial{1, 0, 0};

    std::vector<std::tuple<std::size_t, std::size_t, state>> frontier;
    frontier.emplace_back(px + py, 0, initial);

    std::map<state, std::size_t> dist;

    auto get_erosion = [&](std::size_t i, std::size_t j) {
        while (i >= erosion.size()) {
            erosion.emplace_back(erosion[0].size());
            for (std::size_t k = 0; k < erosion.back().size(); ++k)
                if (k == 0)
                    erosion[i][k] = (X_MULT * i + depth) % BASE;
                else
                    erosion[i][k] = (erosion[i - 1][k] * erosion[i][k - 1] + depth) % BASE;
        }
        while (j >= erosion[0].size()) {
            for (std::size_t k = 0; k < erosion.size(); ++k)
                erosion[k].push_back(
                  ((k == 0 ? Y_MULT * j : erosion[k - 1][j] * erosion[k][j - 1]) + depth) % BASE);
        }
        return erosion[i][j];
    };

    auto get_h = [&](const state& s) {
        std::size_t heuristic = std::abs(long(px - s.x)) + std::abs(long(py - s.y));
        if (s.choice != terminal.choice)
            heuristic += 7;
        return heuristic;
    };

    auto get_successors = [&](const state& s) {
        std::list<std::pair<state, std::size_t>> result;
        if (s.x != 0)
            result.emplace_back(state{s.choice, s.x - 1, s.y}, 1);
        if (s.y != 0)
            result.emplace_back(state{s.choice, s.x, s.y - 1}, 1);
        result.emplace_back(state{s.choice, s.x + 1, s.y}, 1);
        result.emplace_back(state{s.choice, s.x, s.y + 1}, 1);

        for (std::size_t i = 0; i < 3; ++i)
            if (i != s.choice)
                result.emplace_back(state{i, s.x, s.y}, 7);

        result.remove_if([&](const std::pair<state, std::size_t>& r) {
            return get_erosion(r.first.x, r.first.y) % 3 == r.first.choice;
        });
        return result;
    };

    while (!frontier.empty()) {
        std::pop_heap(frontier.begin(), frontier.end(), std::greater<>());
        auto [h, d, curr] = frontier.back();
        frontier.pop_back();
        if (dist.contains(curr))
            continue;

        dist.emplace(curr, d);
        if (curr == terminal)
            break;
        for (auto p : get_successors(curr))
            if (!dist.contains(p.first)) {
                frontier.emplace_back(get_h(p.first) + d + p.second, d + p.second, p.first);
                std::push_heap(frontier.begin(), frontier.end(), std::greater<>());
            }
    }

    std::cout << dist[terminal] << std::endl;
#endif

    return 0;
}

#endif // DAY_22
