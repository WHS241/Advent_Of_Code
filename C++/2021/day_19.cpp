#ifndef DAY_19
#define DAY_19

#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_set>
#include <valarray>
#include <vector>

#include <util/pair_hash.h>

struct beacon {
    int x;
    int y;
    int z;

    beacon() = default;
    beacon(const std::valarray<int>& src) : x(src[0]), y(src[1]), z(src[2]){};
    bool operator==(const beacon& rhs) const = default;
    std::valarray<int> to_pos() const { return std::valarray<int>{x, y, z}; };
};

namespace std {
template<> struct hash<beacon> {
    std::hash<int> field_hasher;
    std::size_t operator()(const beacon& src) const {
        return util::asym_combine_hash(
          util::asym_combine_hash(field_hasher(src.x), field_hasher(src.y)), field_hasher(src.z));
    }
};
}; // namespace std

typedef std::pair<std::size_t, std::size_t> orientation;

static std::array<std::valarray<int>, 6> AXES;

typedef std::valarray<int> vec;

std::size_t get_opposite_axis(std::size_t positive) {
    return (positive == 0 || positive == 5) ? 5 - positive : ((positive + 1) % 4 + 1);
}

std::size_t get_z(const orientation& o) {
    std::valarray<int> z(3);
    z[0] = AXES[o.first][1] * AXES[o.second][2] - AXES[o.second][1] * AXES[o.first][2];
    z[1] = AXES[o.first][2] * AXES[o.second][0] - AXES[o.second][2] * AXES[o.first][0];
    z[2] = AXES[o.first][0] * AXES[o.second][1] - AXES[o.second][0] * AXES[o.first][1];
    return std::find_if(AXES.begin(), AXES.end(), [&z](const auto& v) { return (v == z).min(); })
         - AXES.begin();
}

orientation operator*(const orientation& lhs, const orientation& rhs) {
    std::size_t lhs_z = get_z(lhs);
    vec res_x = AXES[lhs.first] * AXES[rhs.first][0] + AXES[lhs.second] * AXES[rhs.first][1]
              + AXES[lhs_z] * AXES[rhs.first][2];
    vec res_y = AXES[lhs.first] * AXES[rhs.second][0] + AXES[lhs.second] * AXES[rhs.second][1]
              + AXES[lhs_z] * AXES[rhs.second][2];

    orientation result;
    result.first =
      std::find_if(AXES.begin(), AXES.end(), [&res_x](const auto& v) { return (v == res_x).min(); })
      - AXES.begin();
    result.second =
      std::find_if(AXES.begin(), AXES.end(), [&res_y](const auto& v) { return (v == res_y).min(); })
      - AXES.begin();
    return result;
}

std::valarray<int> apply_orient(const orientation& o, const std::valarray<int>& src) {
    std::size_t z = get_z(o);
    return AXES[o.first] * src[0] + AXES[o.second] * src[1] + AXES[z] * src[2];
}

std::pair<std::valarray<int>, std::unordered_set<beacon>>
  find_overlap(const std::unordered_set<beacon>& fixed, const std::unordered_set<beacon>& rotate,
               orientation o) {
    if (o.first >= 6 || o.second >= 6 || o.first == get_opposite_axis(o.second))
        return std::make_pair(std::valarray<int>(), std::unordered_set<beacon>());
    std::unordered_set<beacon> rotated;
    for (const beacon& b : rotate)
        rotated.insert(apply_orient(o, b.to_pos()));

    for (const auto& b_f : fixed) {
        for (const auto& b_r : rotated) {
            bool viable = true;
            vec scan_pos = b_f.to_pos() - b_r.to_pos();
            std::unordered_set<beacon> overlap;
            for (const auto& b_f_2 : fixed) {
                vec rel_to_r = b_f_2.to_pos() - scan_pos;
                if ((rel_to_r <= 1000).min() && (rel_to_r >= -1000).min()) {
                    if (!rotated.contains(rel_to_r)) {
                        viable = false;
                        break;
                    }
                    overlap.insert(b_f_2);
                }
            }
            if (!viable)
                continue;

            for (const auto& b_r_2 : rotated) {
                vec rel_to_f = b_r_2.to_pos() + scan_pos;
                if ((rel_to_f <= 1000).min() && (rel_to_f >= -1000).min()
                    && !fixed.contains(rel_to_f)) {
                    viable = false;
                    break;
                }
            }

            if (viable)
                return std::make_pair(scan_pos, overlap);
        }
    }

    return std::make_pair(std::valarray<int>(), std::unordered_set<beacon>());
}

std::ostream& operator<<(std::ostream& os, const vec& v) {
    return os << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')';
}

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    AXES[0] = {1, 0, 0};
    AXES[1] = {0, 1, 0};
    AXES[2] = {0, 0, 1};
    AXES[3] = {0, -1, 0};
    AXES[4] = {0, 0, -1};
    AXES[5] = {-1, 0, 0};

    std::vector<std::unordered_set<beacon>> scanners;
    for (std::string s; std::getline(reader, s);) {
        if (s.empty())
            continue;
        if (s.back() == '-') {
            scanners.emplace_back();
        } else {
            beacon curr_beacon;
            std::istringstream parser(s);
            std::string num_str;
            std::getline(parser, num_str, ',');
            curr_beacon.x = std::stoi(num_str);
            std::getline(parser, num_str, ',');
            curr_beacon.y = std::stoi(num_str);
            std::getline(parser, num_str);
            curr_beacon.z = std::stoi(num_str);
            scanners.back().insert(curr_beacon);
        }
    }

    std::vector<
      std::vector<std::tuple<orientation, std::valarray<int>, std::unordered_set<beacon>>>>
      overlap(scanners.size());
    for (auto& r : overlap)
        r.resize(scanners.size());

    for (std::size_t i = 0; i < scanners.size(); ++i) {
        for (std::size_t j = i + 1; j < scanners.size(); ++j) {
            for (std::size_t i_vec = 0; i_vec < 6; ++i_vec) {
                for (std::size_t j_vec = 0; j_vec < 6; ++j_vec) {
                    if (i_vec != j_vec && i_vec != get_opposite_axis(j_vec)) {
                        orientation o = {i_vec, j_vec};
                        auto [j_pos, intersect] = find_overlap(scanners[i], scanners[j], o);
                        if (intersect.size() >= 12)
                            overlap[i][j] = std::make_tuple(o, j_pos, intersect);
                    }
                }
            }
        }
    }

    std::vector<std::size_t> history(scanners.size());
    std::vector<std::size_t> processed;
    std::vector<std::size_t> process_order;
    std::vector<bool> finished(scanners.size(), false);
    processed.push_back(0);
    while (!processed.empty()) {
        std::size_t curr = processed.back();
        processed.pop_back();
        if (finished[curr])
            continue;
        process_order.push_back(curr);
        finished[curr] = true;
        for (std::size_t i = 0; i < scanners.size(); ++i) {
            if (!finished[i]) {
                if ((i < curr && !std::get<2>(overlap[i][curr]).empty())
                    || (i > curr && !std::get<2>(overlap[curr][i]).empty())) {
                    history[i] = curr;
                    processed.push_back(i);
                }
            }
        }
    }

    std::unordered_set<beacon> master_list(scanners[0]);
    std::vector<vec> locations(scanners.size());
    locations[0] = {0, 0, 0};
    for (std::size_t i = 1; i < scanners.size(); ++i) {
        std::size_t curr = i;
        std::unordered_set<beacon> curr_pos = scanners[i];
        vec scanner_pos = {0, 0, 0};
        while (curr != 0) {
            std::size_t next = history[curr];
            orientation o;
            vec offset;
            std::unordered_set<beacon> next_coords;
            if (next < curr) {
                std::tie(o, offset, std::ignore) = overlap[next][curr];
                for (auto& b : curr_pos) {
                    beacon transformed(apply_orient(o, b.to_pos()) + offset);
                    next_coords.insert(transformed);
                }
                scanner_pos = apply_orient(o, scanner_pos) + offset;
            } else {
                std::tie(o, offset, std::ignore) = overlap[curr][next];
                orientation inverse;
                bool found_inverse = false;
                for (std::size_t j = 0; !found_inverse && j < 6; ++j) {
                    for (std::size_t k = 0; !found_inverse && k < 6; ++k) {
                        if (j != k && j != get_opposite_axis(k)
                            && orientation{j, k} * o == orientation{0, 1}) {
                            inverse = orientation{j, k};
                            found_inverse = true;
                        }
                    }
                }
                for (auto& b : curr_pos)
                    next_coords.emplace(apply_orient(inverse, b.to_pos() - offset));
                scanner_pos = apply_orient(inverse, scanner_pos - offset);
            }
            curr = next;
            curr_pos = std::move(next_coords);
        }
        for (auto& b : curr_pos)
            master_list.insert(b);
        locations[i] = scanner_pos;
    }

    std::cout << master_list.size() << '\n';
    int max_dist = 0;
    for (std::size_t i = 0; i < locations.size(); ++i)
        for (std::size_t j = i + 1; j < locations.size(); ++j)
            max_dist = std::max(max_dist, std::abs(locations[i][0] - locations[j][0])
                                            + std::abs(locations[i][1] - locations[j][1])
                                            + std::abs(locations[i][2] - locations[j][2]));

    std::cout << max_dist << std::endl;

    return 0;
}

#endif // DAY_19
