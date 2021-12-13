#ifndef DAY_20
#define DAY_20

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <structures/graph.h>

const std::array<std::size_t, 8> REVERSE = {0, 2, 1, 3,
                                            4, 5, 6, 7}; // to undo a call to orient(string, size_t)

std::vector<std::string> orient(const std::vector<std::string>& y, std::size_t setting) {
    if (setting == 0)
        return y;
    // rotate clockwise
    if (setting == 1) {
        return orient(orient(y, 4), 7);
    }

    // rotate counterclockwise
    if (setting == 2) {
        return orient(orient(y, 5), 7);
    }

    // rotate 180
    if (setting == 3) {
        return orient(orient(y, 4), 5);
    }

    // flip on horizontal axis
    if (setting == 4) {
        std::vector<std::string> output(y);
        std::reverse(output.begin(), output.end());
        return output;
    }

    // flip on vertical axis
    if (setting == 5) {
        std::vector<std::string> output(y);
        for (auto& s : output)
            std::reverse(s.begin(), s.end());
        return output;
    }

    // flip UR-DL
    if (setting == 6) {
        return orient(orient(y, 1), 4);
    }

    // flip UL-DR
    if (setting == 7) {
        std::vector<std::string> output(y.size());

        for (std::size_t i = 0; i < y[0].size(); ++i)
            for (std::size_t j = 0; j < y.size(); ++j)
                output[i].push_back(y[j][i]);
        return output;
    }

    return std::vector<std::string>();
}

// determine if two tiles are adjacent
// output: 8 * (edge on x) + 2 * (edge on y) + (reflected ? 1 : 0)
//
// Edges:
// 0: top edge L to R
// 1: bottom edge L to R
// 2: left edge U to D
// 3: right edge U to D
std::size_t matches(const std::vector<std::string>& x, const std::vector<std::string>& y) {
    std::array<std::string, 4> x1, y1;

    x1[0] = x.front();
    x1[1] = x.back();
    for (const auto& p : x) {
        x1[2].push_back(p.front());
        x1[3].push_back(p.back());
    }

    y1[0] = y.front();
    y1[1] = y.back();
    for (const auto& p : y) {
        y1[2].push_back(p.front());
        y1[3].push_back(p.back());
    }

    for (std::size_t i = 0; i < x1.size(); ++i) {
        for (std::size_t j = 0; j < y1.size(); ++j) {
            if (x1[i] == y1[j])
                return 2 * (x1.size() * i + j);
            if (std::equal(x1[i].begin(), x1[i].end(), y1[j].rbegin(), y1[j].rend()))
                return 2 * (x1.size() * i + j) + 1;
        }
    }

    return -1;
}

std::vector<std::string> orient(const std::vector<std::string>& x,
                                const std::vector<std::string>& y) {
    std::array<std::string, 4> x1, y1;

    x1[0] = x.front();
    x1[1] = x.back();
    for (const auto& p : x) {
        x1[2].push_back(p.front());
        x1[3].push_back(p.back());
    }

    y1[0] = y.front();
    y1[1] = y.back();
    for (const auto& p : y) {
        y1[2].push_back(p.front());
        y1[3].push_back(p.back());
    }

    if (x1[1] == y1[0] || x1[3] == y1[2])
        return y;
    if (x1[3] == y1[1] || std::equal(x1[1].begin(), x1[1].end(), y1[2].rbegin(), y1[2].rend()))
        return orient(y, 1);

    if (x1[1] == y1[3] || std::equal(x1[3].begin(), x1[3].end(), y1[0].rbegin(), y1[0].rend()))
        return orient(y, 2);

    if (std::equal(x1[1].begin(), x1[1].end(), y1[1].rbegin(), y1[1].rend())
        || std::equal(x1[3].begin(), x1[3].end(), y1[3].rbegin(), y1[3].rend()))
        return orient(y, 3);

    if (x1[1] == y1[1] || std::equal(x1[3].begin(), x1[3].end(), y1[2].rbegin(), y1[2].rend()))
        return orient(y, 4);

    if (x1[3] == y1[3] || std::equal(x1[1].begin(), x1[1].end(), y1[0].rbegin(), y1[0].rend()))
        return orient(y, 5);

    if (std::equal(x1[1].begin(), x1[1].end(), y1[3].rbegin(), y1[3].rend())
        || std::equal(x1[3].begin(), x1[3].end(), y1[1].rbegin(), y1[1].rend()))
        return orient(y, 6);

    if (x1[1] == y1[2] || x1[3] == y1[0])
        return orient(y, 7);

    return std::vector<std::string>();
}

int day_20_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    graph::graph<std::size_t, true, true, std::size_t> connected_ids;
    std::unordered_map<std::size_t, std::vector<std::string>> tiles;

    // determine which tiles are adjacent to each other; edge "weight" is used to determine
    // orientation
    for (std::string line; std::getline(reader, line);) {
        std::size_t id = std::stoul(line.substr(5));
        connected_ids.add_vertex(id);

        while (std::getline(reader, line) && !line.empty())
            tiles[id].push_back(line);
        for (std::size_t v : connected_ids.vertices()) {
            std::size_t match_cat = matches(tiles[id], tiles[v]);
            if (id != v && match_cat != -1) {
                connected_ids.force_add(id, v, match_cat);
                connected_ids.force_add(
                  v, id, 2 * ((match_cat / 2 % 4) * 4 + match_cat / 8) + match_cat % 2);
            }
        }
    }

    std::vector<std::size_t> corners;
    for (std::size_t v : connected_ids.vertices())
        if (connected_ids.degree(v) == 2)
            corners.push_back(v);

    std::copy(corners.begin(), corners.end(), std::ostream_iterator<std::size_t>(std::cout, "\n"));
    std::cout << std::accumulate(corners.begin(), corners.end(), 1UL, std::multiplies<>()) << '\n';

    // Part 2
    // Generate the tile grid
    std::vector<std::vector<std::size_t>> picture;
    std::size_t x_dim(connected_ids.order()), y_dim(connected_ids.order());
    std::unordered_set<std::size_t> processed;
    for (std::size_t i = 0; i < x_dim; ++i) {
        picture.emplace_back();
        for (std::size_t j = 0; j < y_dim; ++j) {
            if (i == 0 && j == 0) {
                // pick an arbitrary corner for the top-left corner
                picture[i].push_back(corners.front());
                processed.insert(corners.front());
                continue;
            }
            if (j == 0) {
                // we can pick either neighbor for picture[0][1], and this uniquely determines the
                // entire grid layout
                std::list<std::size_t> candidates = connected_ids.neighbors(picture[i - 1][j]);
                candidates.remove_if([&](std::size_t x) {
                    return processed.contains(x) || connected_ids.degree(x) == 4;
                });
                picture.back().push_back(candidates.front());
                if (connected_ids.degree(candidates.front()) == 2)
                    x_dim = picture.size();
                processed.insert(candidates.front());
            } else {
                std::list<std::size_t> candidates = connected_ids.neighbors(picture[i][j - 1]);

                candidates.remove_if([&](std::size_t x) {
                    if (processed.contains(x))
                        return true;

                    if (i == 0)
                        return connected_ids.degree(x) == 4;

                    std::list<std::size_t> current_neighbors = connected_ids.neighbors(x);
                    return std::find(current_neighbors.begin(), current_neighbors.end(),
                                     picture[i - 1][j])
                        == current_neighbors.end();
                });
                picture.back().push_back(candidates.front());
                processed.insert(candidates.front());
                if (connected_ids.degree(candidates.front()) == 2)
                    y_dim = picture.back().size();
            }
        }
    }

    std::vector<std::string> layout(x_dim * (tiles.begin()->second.size() - 2));
    for (std::size_t i = 0; i < x_dim; ++i) {
        for (std::size_t j = 0; j < y_dim; ++j) {
            std::vector<std::string>& sec = tiles[picture[i][j]];

            // orient based on already set tiles
            if (i != 0) {
                sec = orient(tiles[picture[i - 1][j]], sec);
            } else if (j != 0) {
                sec = orient(tiles[picture[i][j - 1]], sec);
            } else {
                // orient top left corner based on which edges connect
                std::size_t link_type_r = connected_ids.edge_cost(picture[0][0], picture[0][1]),
                            link_type_d = connected_ids.edge_cost(picture[0][0], picture[1][0]);
                if (link_type_r / 8 == 0) {
                    if (link_type_d / 8 == 3)
                        sec = orient(sec, 1);
                    else if (link_type_d / 8 == 2)
                        sec = orient(sec, 6);
                    else
                        std::cout << "ERROR: r = 0" << std::endl;
                } else if (link_type_r / 8 == 1) {
                    if (link_type_d / 8 == 2)
                        sec = orient(sec, 2);
                    else if (link_type_d / 8 == 3)
                        sec = orient(sec, 7);
                    else
                        std::cout << "ERROR: r = 1" << std::endl;
                } else if (link_type_r / 8 == 2) {
                    if (link_type_d / 8 == 0)
                        sec = orient(sec, 3);
                    else if (link_type_d / 8 == 1)
                        sec = orient(sec, 5);
                    else
                        std::cout << "ERROR: r = 2" << std::endl;
                } else if (link_type_r / 8 == 3) {
                    if (link_type_d / 8 == 0)
                        sec = orient(sec, 4);
                    else if (link_type_d / 8 == 0)
                        sec = orient(sec, 0);
                    else
                        std::cout << "ERROR: r = 3" << std::endl;
                }
            }

            for (std::size_t k = 1; k < sec.size() - 1; ++k)
                layout[i * (sec.size() - 2) + k - 1] += sec[k].substr(1, sec.size() - 2);
        }
    }

    std::vector<std::string> sea_monster = {"                  # ", "#    ##    ##    ###",
                                            " #  #  #  #  #  #   "};

    std::size_t num_monsters = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        layout = orient(layout, i);
        for (std::size_t j = 0; j < layout.size() - sea_monster.size() + 1; ++j) {
            for (std::size_t k = 0; k < layout[0].size() - sea_monster[0].size() + 1; ++k) {
                bool found_dragon = true;
                for (std::size_t a = 0; found_dragon && a < sea_monster.size(); ++a)
                    for (std::size_t b = 0; found_dragon && b < sea_monster[0].size(); ++b)
                        if (sea_monster[a][b] == '#' && layout[j + a][k + b] != '#'
                            && layout[j + a][k + b] != 'O')
                            found_dragon = false;

                if (found_dragon) {
                    ++num_monsters;
                    for (std::size_t a = 0; a < sea_monster.size(); ++a)
                        for (std::size_t b = 0; b < sea_monster[0].size(); ++b)
                            if (sea_monster[a][b] == '#')
                                layout[j + a][k + b] = 'O';
                }
            }
        }

        layout = orient(layout, REVERSE[i]);
    }

    std::cout << std::transform_reduce(
      layout.begin(), layout.end(), 0UL, std::plus<>(), [](const std::string& s) {
          return std::count(s.begin(), s.end(), '#');
      }) << std::endl;

    return 0;
}

#endif // DAY_20
