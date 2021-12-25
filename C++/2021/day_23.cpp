#ifndef DAY_23
#define DAY_23

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <queue>
#include <unordered_set>
#include <vector>

#include <util/pair_hash.h>

namespace std {
template<> struct hash<std::vector<std::string>> {
    std::hash<std::string> hasher;

    std::size_t operator()(const std::vector<std::string>& v) const {
        return std::accumulate(v.begin(), v.end(), 0UL,
                               [this](std::size_t p, const std::string& c) {
                                   return util::asym_combine_hash(p, hasher(c));
                               });
    }
};
} // namespace std

using frontier_t = std::pair<std::size_t, std::vector<std::string>>;

std::size_t calc_heuristic(const std::vector<std::string>& curr) {
    std::size_t result = 0;

    // rooms
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 1; j <= curr.size() - 3; ++j) {
            if (std::isalpha(curr[j + 1][2 * i + 3]) && curr[j + 1][2 * i + 3] - 'A' != i) {
                std::size_t dist_to_dest = j + 1 + std::abs(2 * (curr[j + 1][2 * i + 3] - 'A'));
                for (std::size_t k = 0; k < curr[j + 1][2 * i + 3] - 'A'; ++k)
                    dist_to_dest *= 10;
                result += dist_to_dest;
            }
        }
    }

    // hallway
    for (long i = 0; i < curr[1].size(); ++i) {
        if (std::isalpha(curr[1][i])) {
            long target_col = 2 * (curr[1][i] - 'A') * 3;
            std::size_t dist_to_dest = 1 + std::abs(target_col - i);
            for (std::size_t k = 0; k < curr[1][i] - 'A'; ++k)
                dist_to_dest *= 10;
            result += dist_to_dest;
        }
    }

    return result;
}

bool is_terminal(const std::vector<std::string>& v) {
    for (int i = 0; i < 4; ++i)
        for (int j = 2; j < v.size() - 1; ++j)
            if (v[j][2 * i + 3] != 'A' + i)
                return false;
    return true;
}

bool room_completed(const std::vector<std::string>& v, char c) {
    for (std::size_t j = 2; j < v.size() - 1; ++j)
        if (v[j][2 * (c - 'A') + 3] != c)
            return false;
    return true;
}

bool ready_for_push(const std::vector<std::string>& v, char c) {
    for (std::size_t j = 2; j < v.size() - 1; ++j)
        if (v[j][2 * (c - 'A') + 3] != c && v[j][2 * (c - 'A') + 3] != '.')
            return false;
    return true;
}

std::size_t find_bottom(const std::vector<std::string>& v, std::size_t i) {
    for (std::size_t j = 1; j < v.size() - 1; ++j)
        if (v[j + 1][i] != '.')
            return j;
    return 0;
}

std::list<frontier_t> get_successors(const frontier_t& curr) {
    std::list<frontier_t> result;
    for (long i = 0; i < curr.second[1].size(); ++i) {
        if (std::isalpha(curr.second[1][i])) {
            long target_col = 2 * (curr.second[1][i] - 'A') + 3;
            long lower, upper;
            if (i < target_col) {
                lower = i + 1;
                upper = target_col + 1;
            } else {
                lower = target_col;
                upper = i;
            }
            if (ready_for_push(curr.second, curr.second[1][i])
                && std::all_of(curr.second[1].begin() + lower, curr.second[1].begin() + upper,
                               [](char c) { return c == '.'; })) {
                std::size_t depth = find_bottom(curr.second, target_col);
                if (depth > 1) {
                    std::size_t cost = std::abs(target_col - i) + depth - 1;

                    for (char c = 'A'; c != curr.second[1][i]; ++c)
                        cost *= 10;
                    frontier_t next{curr.first + cost, curr.second};
                    next.second[1][i] = '.';
                    next.second[depth][target_col] = curr.second[1][i];
                    result.push_back(std::move(next));
                }
            }
        }
    }

    for (char c = 'A'; c <= 'D'; ++c) {
        if (!ready_for_push(curr.second, c)) {
            std::size_t pop_col = 2 * (c - 'A') + 3;
            std::size_t pop_src = find_bottom(curr.second, pop_col) + 1;
            for (std::size_t j = pop_col - 1; curr.second[1][j] == '.'; --j) {
                if (curr.second[2][j] == '#') {
                    std::size_t cost = pop_col + pop_src - (1 + j);
                    for (char c2 = 'A'; c2 != curr.second[pop_src][pop_col]; ++c2)
                        cost *= 10;
                    frontier_t next{curr.first + cost, curr.second};
                    next.second[pop_src][pop_col] = '.';
                    next.second[1][j] = curr.second[pop_src][pop_col];
                    result.push_back(std::move(next));
                }
            }
            for (std::size_t j = pop_col + 1; curr.second[1][j] == '.'; ++j) {
                if (curr.second[2][j] == '#') {
                    std::size_t cost = j + pop_src - (1 + pop_col);
                    for (char c2 = 'A'; c2 != curr.second[pop_src][pop_col]; ++c2)
                        cost *= 10;
                    frontier_t next{curr.first + cost, curr.second};
                    next.second[pop_src][pop_col] = '.';
                    next.second[1][j] = curr.second[pop_src][pop_col];
                    result.push_back(std::move(next));
                }
            }
        }
        /*
        if (std::isalpha(curr.second[2][col]) && (curr.second[2][col] != 'A' + i ||
        curr.second[3][col] != 'A' + i)) { for (int j = col - 1; curr.second[1][j] == '.'; --j) { if
        (curr.second[2][j] == '#') { std::size_t cost = col - j + 1; for (char c = 'A'; c !=
        curr.second[2][col]; ++c) cost *= 10; frontier_t next{curr.first + cost, curr.second};
                    next.second[2][col] = '.';
                    next.second[1][j] = curr.second[2][col];
                    result.push_back(std::move(next));
                }
            }
            for (int j = col + 1; curr.second[1][j] == '.'; ++j) {
                if (curr.second[2][j] == '#') {
                    std::size_t cost = j - col + 1;
                    for (char c = 'A'; c != curr.second[2][col]; ++c)
                        cost *= 10;
                    frontier_t next{curr.first + cost, curr.second};
                    next.second[2][col] = '.';
                    next.second[1][j] = curr.second[2][col];
                    result.push_back(std::move(next));
                }
            }
        }
        if (curr.second[2][col] == '.' && std::isalpha(curr.second[3][col]) && curr.second[3][col]
        != 'A' + i) { for (int j = col - 1; curr.second[1][j] == '.'; --j) { if (curr.second[2][j]
        == '#') { std::size_t cost = col - j + 2; for (char c = 'A'; c != curr.second[3][col]; ++c)
                        cost *= 10;
                    frontier_t next{curr.first + cost, curr.second};
                    next.second[3][col] = '.';
                    next.second[1][j] = curr.second[3][col];
                    result.push_back(std::move(next));
                }
            }
            for (int j = col + 1; curr.second[1][j] == '.'; ++j) {
                if (curr.second[2][j] == '#') {
                    std::size_t cost = j - col + 2;
                    for (char c = 'A'; c != curr.second[3][col]; ++c)
                        cost *= 10;
                    frontier_t next{curr.first + cost, curr.second};
                    next.second[3][col] = '.';
                    next.second[1][j] = curr.second[3][col];
                    result.push_back(std::move(next));
                }
            }
        }*/
    }

    return result;
}

int day_23_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> input;
    for (std::string s; std::getline(reader, s);)
        input.push_back(s);

    auto compare = [](const frontier_t& v1, const frontier_t& v2) {
        return v1.first > v2.first;
    };

    std::priority_queue<frontier_t, std::vector<frontier_t>, decltype(compare)> frontier(compare);
    std::unordered_map<std::vector<std::string>, std::size_t> visited;
    std::unordered_map<std::vector<std::string>, std::vector<std::string>> from;

    std::size_t final_cost = 0;
    bool finished = false;

    frontier.emplace(0, input);
    while (!finished) {
        frontier_t curr = frontier.top();
        frontier.pop();
        if (!visited.contains(curr.second)) {
            visited[curr.second] = curr.first;

            if (is_terminal(curr.second)) {
                final_cost = curr.first;
                finished = true;
            } else {
                for (const frontier_t& next : get_successors(curr))
                    if (!visited.contains(next.second))
                        frontier.push(next);
            }
        }
    }

    std::cout << final_cost << std::endl;

    return 0;
}

#endif // DAY_23
