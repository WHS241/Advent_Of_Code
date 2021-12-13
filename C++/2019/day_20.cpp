#ifndef DAY_20
#define DAY_20

#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <util/pair_hash.h>

int day_20_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::vector<std::string> maze;
    std::ifstream reader(argv[1]);

    for (std::string line; std::getline(reader, line);)
        maze.push_back(line);

    std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> initial_coords;

    std::unordered_map<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>,
                       util::pair_hash<std::size_t, std::size_t>>
      equiv_coords;

    std::pair<std::size_t, std::size_t> start, end, inner_hole_nw(0, 0), inner_hole_se(0, 0);

    std::string START = "AA";
    std::string END = "ZZ";

    auto get_portal_coord = [&maze](const std::pair<std::size_t, std::size_t>& border) {
        std::array<std::pair<std::size_t, std::size_t>, 4> to_try = {
          std::make_pair(border.first + 1, border.second),
          std::make_pair(border.first - 1, border.second),
          std::make_pair(border.first, border.second + 1),
          std::make_pair(border.first, border.second - 1)};

        for (const std::pair<std::size_t, std::size_t>& x : to_try)
            if (std::isalpha(maze[x.first][x.second]))
                return x;
        throw std::runtime_error("Not Portal");
    };

    auto handle_portal = [&](const std::string& portal,
                             const std::pair<std::size_t, std::size_t>& coords) {
        if (portal == START)
            start = coords;
        else if (portal == END)
            end = coords;
        else if (initial_coords.find(portal) == initial_coords.end())
            initial_coords[portal] = coords;
        else {
            equiv_coords[get_portal_coord(coords)] = initial_coords[portal];
            equiv_coords[get_portal_coord(initial_coords[portal])] = coords;
        }
    };

    // Upper edge
    for (std::size_t i = 2; i < maze[0].size() - 2; ++i) {
        if (std::isalpha(maze[0][i])) {
            std::string portal;
            portal.push_back(maze[0][i]);
            portal.push_back(maze[1][i]);
            handle_portal(portal, std::make_pair(2, i));
        }
    }

    // Bottom edge
    for (std::size_t i = 2; i < maze[maze.size() - 2].size(); ++i) {
        if (std::isalpha(maze[maze.size() - 2][i])) {
            std::string portal;
            portal.push_back(maze[maze.size() - 2][i]);
            portal.push_back(maze[maze.size() - 1][i]);
            handle_portal(portal, std::make_pair(maze.size() - 3, i));
        }
    }

    // Middle
    for (std::size_t i = 2; i < maze.size() - 2; ++i) {
        // Portal on left?
        if (std::isalpha(maze[i][0])) {
            std::string portal;
            portal.push_back(maze[i][0]);
            portal.push_back(maze[i][1]);
            handle_portal(portal, std::make_pair(i, 2));
        }

        // Portal on right?
        if (std::isalpha(maze[i][maze[i].size() - 2])) {
            std::string portal;
            portal.push_back(maze[i][maze[i].size() - 2]);
            portal.push_back(maze[i][maze[i].size() - 1]);
            handle_portal(portal, std::make_pair(i, maze[i].size() - 3));
        }

        // Should we look for gap?
        if (inner_hole_nw == std::make_pair(0UL, 0UL)) {
            for (std::size_t j = 2; j < maze[i].size() - 2; ++j)
                if (std::isspace(maze[i][j])) {
                    inner_hole_nw = {i, j};
                    break;
                }

            // Try to find the southeast corner as well if there is a hole
            if (inner_hole_nw != std::make_pair(0UL, 0UL)) {
                for (std::size_t j = inner_hole_nw.second; j < maze[i].size() - 2; ++j) {
                    if (std::isspace(maze[i][j]) || std::isalpha(maze[i][j]))
                        inner_hole_se.second = j;
                    else
                        break;
                }

                for (std::size_t j = i; j < maze.size() - 2; ++j) {
                    if (std::isspace(maze[j][inner_hole_nw.second])
                        || std::isalpha(maze[j][inner_hole_nw.second]))
                        inner_hole_se.first = j;
                    else
                        break;
                }
            }
        }

        // Account for hole if overlaps with current row
        if (inner_hole_nw != std::make_pair(0UL, 0UL) && i <= inner_hole_se.first) {
            if (i == inner_hole_nw.first) {
                // Upper edge
                for (std::size_t j = inner_hole_nw.second; j <= inner_hole_se.second; ++j) {
                    if (std::isalpha(maze[i][j])) {
                        std::string portal;
                        portal.push_back(maze[i][j]);
                        portal.push_back(maze[i + 1][j]);
                        handle_portal(portal, std::make_pair(i - 1, j));
                    }
                }
            } else if (i == inner_hole_se.first) {
                // Lower edge
                for (std::size_t j = inner_hole_nw.second; j <= inner_hole_se.second; ++j) {
                    if (std::isalpha(maze[i][j])) {
                        std::string portal;
                        portal.push_back(maze[i - 1][j]);
                        portal.push_back(maze[i][j]);
                        handle_portal(portal, std::make_pair(i + 1, j));
                    }
                }
            } else {
                // Side
                if (std::isalpha(maze[i][inner_hole_nw.second])) {
                    std::string portal;
                    portal.push_back(maze[i][inner_hole_nw.second]);
                    portal.push_back(maze[i][inner_hole_nw.second + 1]);
                    handle_portal(portal, std::make_pair(i, inner_hole_nw.second - 1));
                }
                if (std::isalpha(maze[i][inner_hole_se.second])) {
                    std::string portal;
                    portal.push_back(maze[i][inner_hole_se.second - 1]);
                    portal.push_back(maze[i][inner_hole_se.second]);
                    handle_portal(portal, std::make_pair(i, inner_hole_se.second + 1));
                }
            }
        }
    }

    // BFS for shortest path
    std::list<std::tuple<std::size_t, std::pair<std::size_t, std::size_t>, std::size_t>> frontier;
    std::vector<std::vector<std::unordered_set<std::size_t>>> visited(
      maze.size(), std::vector<std::unordered_set<std::size_t>>(maze.size()));
    frontier.emplace_back(0UL, start, 0UL);
    visited[start.first][start.second];
    while (!frontier.empty()) {
        std::size_t level;
        std::pair<std::size_t, std::size_t> coords;
        std::size_t dist;
        std::tie(level, coords, dist) = frontier.front();
        frontier.pop_front();

        if (coords == end && level == 0) {
            std::cout << dist << std::endl;
            break;
        }

        std::array<std::pair<std::size_t, std::size_t>, 4> neighbors = {
          std::make_pair(coords.first + 1, coords.second),
          std::make_pair(coords.first - 1, coords.second),
          std::make_pair(coords.first, coords.second - 1),
          std::make_pair(coords.first, coords.second + 1)};

        std::array<std::size_t, 4> next_level = {level, level, level, level};

        for (std::size_t i = 0; i < neighbors.size(); ++i) {
            std::pair<std::size_t, std::size_t>& n = neighbors[i];
            if (equiv_coords.find(n) != equiv_coords.end()) {
                if (n.first == 1 || n.second == 1 || n.first == maze.size() - 2
                    || n.second == maze[0].size() - 2) {
                    --next_level[i];
                } else {
                    ++next_level[i];
                }
                n = equiv_coords[n];
            }
        }

        for (std::size_t i = 0; i < neighbors.size(); ++i) {
            std::pair<std::size_t, std::size_t>& n = neighbors[i];
            if (maze[n.first][n.second] == '.' && next_level[i] != -1UL
                && visited[n.first][n.second].count(next_level[i]) == 0) {
                visited[n.first][n.second].insert(next_level[i]);
                frontier.emplace_back(next_level[i], n, dist + 1);
            }
        }
    }

    return 0;
}

#endif // DAY_20
