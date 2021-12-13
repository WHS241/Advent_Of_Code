#ifndef DAY_12
#define DAY_12 2

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <tuple>
#include <unordered_set>
#include <vector>

#include "util/pair_hash.h"

static constexpr std::size_t NUM_DIM = 3;

void advance(std::vector<std::array<int, NUM_DIM>>& positions,
             std::vector<std::array<int, NUM_DIM>>& vels) {
    for (std::size_t i = 0; i < positions.size(); ++i) {
        for (std::size_t j = i + 1; j < positions.size(); ++j) {
            for (std::size_t k = 0; k < NUM_DIM; ++k) {
                if (positions[i][k] < positions[j][k]) {
                    ++vels[i][k];
                    --vels[j][k];
                } else if (positions[i][k] > positions[j][k]) {
                    --vels[i][k];
                    ++vels[j][k];
                }
            }
        }

        for (std::size_t j = 0; j < NUM_DIM; ++j)
            positions[i][j] += vels[i][j];
    }
}

int day_12_main(int, char**) {
    std::vector<std::array<int, 3>> position, velocity;

    position.push_back({-9, 10, -1});
    position.push_back({-14, -8, 14});
    position.push_back({1, 5, 6});
    position.push_back({-19, 7, 8});

    while (velocity.size() != position.size())
        velocity.push_back({0, 0, 0});

    struct vector_hash {
        std::size_t operator()(const std::vector<int>& values) const {
            std::hash<int> hasher;
            if (values.empty())
                return hasher(0);
            std::size_t hash = hasher(values.front());
            for (std::size_t i = 1; i < values.size(); ++i)
                hash = util::asym_combine_hash(hash, hasher(values[i]));
            return hash;
        }
    };

#if DAY_12 == 2
    std::array<std::unordered_set<std::vector<int>, vector_hash>, NUM_DIM> occured;
    std::array<bool, NUM_DIM> halted;
    for (std::size_t i = 0; i < NUM_DIM; ++i)
        halted[i] = false;

    while (std::any_of(halted.begin(), halted.end(), [](bool b) { return !b; })) {
        advance(position, velocity);

        for (std::size_t i = 0; i < NUM_DIM; ++i) {
            if (!halted[i]) {
                std::vector<int> aggreg_coord;
                aggreg_coord.reserve(2 * position.size());
                for (std::size_t j = 0; j < position.size(); ++j) {
                    aggreg_coord.push_back(position[j][i]);
                    aggreg_coord.push_back(velocity[j][i]);
                }

                std::size_t map_size = occured[i].size();
                occured[i].insert(aggreg_coord);
                if (map_size == occured[i].size())
                    halted[i] = true;
            }
        }
    }

    std::size_t common = occured[0].size();
    for (std::size_t i = 1; i < NUM_DIM; ++i)
        common = std::lcm(common, occured[i].size());

    std::cout << common << std::endl;
#endif

    return 0;
}

#endif // DAY_12
