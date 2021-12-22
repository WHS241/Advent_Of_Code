#ifndef DAY_22
#define DAY_22

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <sstream>

struct cuboid {
    long min_x, max_x, min_y, max_y, min_z, max_z;

    bool operator==(const cuboid&) const = default;

    long get_volume() const {
        return (max_x - min_x + 1) * (max_y - min_y + 1) * (max_z - min_z + 1);
    }

    bool valid() const { return min_x <= max_x && min_y <= max_y && min_z <= max_z; }
};

struct command {
    cuboid region;
    bool turn_on;
};

cuboid generate_neighbor(const cuboid& created, const cuboid& region, int dx, int dy, int dz) {
    cuboid result = created;
    if (dx != 0) {
        result.min_y = region.min_y;
        result.max_y = region.max_y;
        result.min_z = region.min_z;
        result.max_z = region.max_z;
        if (dx > 0) {
            result.min_x = result.max_x + 1;
            result.max_x = region.max_x;
        } else {
            result.max_x = result.min_x - 1;
            result.min_x = region.min_x;
        }
        return result;
    }

    if (dy != 0) {
        result.min_z = region.min_z;
        result.max_z = region.max_z;
        if (dy > 0) {
            result.min_y = result.max_y + 1;
            result.max_y = region.max_y;
        } else {
            result.max_y = result.min_y - 1;
            result.min_y = region.min_y;
        }
        return result;
    }

    if (dz > 0) {
        result.min_z = result.max_z + 1;
        result.max_z = region.max_z;
    } else if (dz < 0) {
        result.max_z = result.min_z - 1;
        result.min_z = region.min_z;
    }

    return result;
}

std::list<cuboid> set_minus(const cuboid& c1, const cuboid& c2) {
    cuboid intersection;
    intersection.min_x = std::max(c1.min_x, c2.min_x);
    intersection.max_x = std::min(c1.max_x, c2.max_x);
    intersection.min_y = std::max(c1.min_y, c2.min_y);
    intersection.max_y = std::min(c1.max_y, c2.max_y);
    intersection.min_z = std::max(c1.min_z, c2.min_z);
    intersection.max_z = std::min(c1.max_z, c2.max_z);

    if (!intersection.valid())
        return std::list<cuboid>{c1};
    if (intersection == c1)
        return std::list<cuboid>{};
    std::list<cuboid> result;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                if ((x || y || z) && ((!x && !y) || (!y && !z) || (!x && !z))) {
                    cuboid next = generate_neighbor(intersection, c1, x, y, z);
                    if (next.valid())
                        result.push_back(next);
                }
            }
        }
    }

    return result;
}

int day_22_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);

    int part = std::stoi(argv[2]);
    if (part != 1 && part != 2)
        return 1;

    std::list<command> commands;

    for (std::string s; std::getline(reader, s);) {
        std::istringstream parser(s);
        std::string direction;
        std::getline(parser, direction, ' ');
        std::string x_command, y_command, z_command;
        std::getline(parser, x_command, ',');
        std::getline(parser, y_command, ',');
        std::getline(parser, z_command, ',');

        command curr;
        curr.turn_on = direction == "on";
        curr.region.min_x = std::stol(x_command.substr(2, x_command.find_first_of('.') - 2));
        curr.region.max_x = std::stol(x_command.substr(1 + x_command.find_last_of('.')));
        curr.region.min_y = std::stol(y_command.substr(2, y_command.find_first_of('.') - 2));
        curr.region.max_y = std::stol(y_command.substr(1 + y_command.find_last_of('.')));
        curr.region.min_z = std::stol(z_command.substr(2, z_command.find_first_of('.') - 2));
        curr.region.max_z = std::stol(z_command.substr(1 + z_command.find_last_of('.')));

        if (part == 1) {
            curr.region.min_x = std::max(curr.region.min_x, -50L);
            curr.region.max_x = std::min(curr.region.max_x, 50L);
            curr.region.min_y = std::max(curr.region.min_y, -50L);
            curr.region.max_y = std::min(curr.region.max_y, 50L);
            curr.region.min_z = std::max(curr.region.min_z, -50L);
            curr.region.max_z = std::min(curr.region.max_z, 50L);
        }

        commands.push_back(curr);
    }

    if (part == 1)
        std::erase_if(commands, [](const command& c) { return !c.region.valid(); });

    std::list<cuboid> on_regions;

    for (const command& c : commands) {
        if (c.turn_on) {
            std::list<cuboid> new_regions{c.region};
            for (const cuboid& existing : on_regions) {
                std::list<cuboid> set_minus_res;
                for (const cuboid& target : new_regions)
                    set_minus_res.splice(set_minus_res.end(), set_minus(target, existing));
                new_regions = std::move(set_minus_res);
            }
            on_regions.splice(on_regions.end(), new_regions);
        } else {
            std::list<cuboid> next_regions;
            for (const cuboid& on_reg : on_regions) {
                next_regions.splice(next_regions.end(), set_minus(on_reg, c.region));
            }
            on_regions = std::move(next_regions);
        }
    }

    std::cout << std::transform_reduce(on_regions.begin(), on_regions.end(), 0L, std::plus<>(),
                                       std::mem_fn(&cuboid::get_volume))
              << std::endl;

    return 0;
}

#endif // DAY_22
