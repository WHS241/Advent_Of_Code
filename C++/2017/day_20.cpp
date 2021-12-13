#ifndef DAY_20
#define DAY_20 2

#include <util/it_hash.h>
#include <util/pair_hash.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <vector>

typedef std::array<std::array<long, 3>, 3> state;

#if DAY_20 == 1
bool compare_trajectories(const state& p1, const state& p2) {
    // 2p(t) = 2p0 + (2v0 + a) t + a t^2
    std::array<std::array<long, 3>, 3> quad1, quad2; // coordinate, then power
    for (std::size_t i = 0; i < quad1.size(); ++i) {
        quad1[i][0] = 2 * p1[0][i];
        quad1[i][1] = 2 * p1[1][i] + p1[2][i];
        quad1[i][2] = p1[2][i];
        quad2[i][0] = 2 * p2[0][i];
        quad2[i][1] = 2 * p2[1][i] + p2[2][i];
        quad2[i][2] = p2[2][i];
    }

    // Orient all in the positive direction
    for (std::size_t i = 0; i < 3; ++i) {
        if (quad1[i][2] < 0 ||
            (quad1[i][2] == 0 && (quad1[i][1] < 0 || (quad1[i][1] == 0 && quad1[i][0] < 0))))
            for (std::size_t j = 0; j < 3; ++j)
                quad1[i][j] *= -1;

        if (quad2[i][2] < 0 ||
            (quad2[i][2] == 0 && (quad2[i][1] < 0 || (quad2[i][1] == 0 && quad2[i][0] < 0))))
            for (std::size_t j = 0; j < 3; ++j)
                quad2[i][j] *= -1;
    }

    // Manhattan distance in long term, quadratic function
    std::array<long, 3> dist_q1, dist_q2;
    std::fill(dist_q1.begin(), dist_q1.end(), 0L);
    std::fill(dist_q2.begin(), dist_q2.end(), 0L);

    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            dist_q1[i] += quad1[j][i];
            dist_q2[i] += quad2[j][i];
        }
    }

    std::reverse(dist_q1.begin(), dist_q1.end());
    std::reverse(dist_q2.begin(), dist_q2.end());

    return dist_q1 < dist_q2;
}
#elif DAY_20 == 2
bool may_collide(const state& x, const state& y) {
    state relative;
    for (std::size_t i = 0; i < x.size(); ++i)
        for (std::size_t j = 0; j < x[i].size(); ++j)
            relative[i][j] = x[i][j] - y[i][j];
    return std::inner_product(relative[1].begin(), relative[1].end(), relative[2].begin(), 0L) <
             0 ||
           std::inner_product(relative[0].begin(), relative[0].end(), relative[1].begin(), 0L) < 0;
}

struct pos_hash {
    std::size_t operator()(const state& s) const {
        static std::hash<long> hasher;
        return std::accumulate(s[0].begin(), s[0].end(), 0UL, [](std::size_t h, long s) {
            return util::asym_combine_hash(h, hasher(s));
        });
    }
};

struct pos_eq {
    bool operator()(const state& x, const state& y) const { return x[0] == y[0]; }
};

std::vector<state> step_sim(const std::vector<state>& curr) {
    std::unordered_set<state, pos_hash, pos_eq> particles;
    typedef std::unordered_set<state, pos_hash, pos_eq>::iterator set_it;
    std::unordered_set<set_it, util::it_hash<set_it>> to_remove;

    for (const state& s : curr) {
        state next(s);
        for (std::size_t i = 0; i < 3; ++i) {
            next[1][i] += next[2][i];
            next[0][i] += next[1][i];
        }

        std::pair<set_it, bool> ins = particles.insert(next);
        if (!ins.second)
            to_remove.insert(ins.first);
    }

    for (set_it it : to_remove)
        particles.erase(it);

    return std::vector<state>(particles.begin(), particles.end());
}
#endif

int day_20_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<state> particles;

    for (std::string line; std::getline(reader, line);) {
        particles.emplace_back();
        std::istringstream line_read(line);
        for (std::array<long, 3>& v : particles.back()) {
            line_read.ignore(std::numeric_limits<std::streamsize>::max(), '<');
            std::string vec_string;
            std::getline(line_read, vec_string, '>');
            std::size_t delim = 0;
            for (std::size_t i = 0; i < v.size(); ++i) {
                std::size_t next_pos = vec_string.find(',', delim);
                v[i]                 = std::stol(vec_string.substr(delim, next_pos - delim));
                delim                = next_pos + 1;
            }
        }
    }

#if DAY_20 == 1
    auto min_it = std::min_element(particles.begin(), particles.end(), compare_trajectories);
    std::cout << min_it - particles.begin() << std::endl;
#elif DAY_20 == 2
    bool potential = true;
    while (potential) {
        particles = step_sim(particles);
        potential = false;

        for (std::size_t i = 0; !potential && i < particles.size(); ++i) {
            for (std::size_t j = i + 1; !potential && j < particles.size(); ++j) {
                if (may_collide(particles[i], particles[j])) {
                    potential = true;
                }
            }
        }
    }

    std::cout << particles.size() << std::endl;
#endif

    return 0;
}

#endif // DAY_20
