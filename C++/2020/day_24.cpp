#ifndef DAY_24
#define DAY_24

#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_set>

#include <util/pair_hash.h>

std::array<std::pair<long, long>, 6> get_neighbors(const std::pair<long, long>& x) {
    return {std::make_pair(x.first - 2, x.second),     std::make_pair(x.first + 2, x.second),
            std::make_pair(x.first - 1, x.second - 1), std::make_pair(x.first - 1, x.second + 1),
            std::make_pair(x.first + 1, x.second - 1), std::make_pair(x.first + 1, x.second + 1)};
}

int day_24_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_set<std::pair<long, long>, util::pair_hash<long, long>> flipped;
    for (std::string line; std::getline(reader, line);) {
        long x(0), y(0);
        for (char c : line) {
            switch (c) {
            case 'n':
                ++y;
                break;

            case 's':
                --y;
                break;

            case 'e':
                if (std::abs(y) % 2 == std::abs(x) % 2)
                    x += 2;
                else
                    ++x;
                break;

            case 'w':
                if (std::abs(y) % 2 == std::abs(x) % 2)
                    x -= 2;
                else
                    --x;
                break;
            }
        }
        if (flipped.contains({x, y}))
            flipped.erase({x, y});
        else
            flipped.emplace(x, y);
    }

    std::cout << flipped.size() << std::endl;

    // PART 2
    if (argc < 3)
        return 0;

    std::size_t num_cycles = std::stoul(argv[2]);

    for (std::size_t i = 0; i < num_cycles; ++i) {
        std::unordered_set<std::pair<long, long>, util::pair_hash<long, long>> next;
        for (const auto& p : flipped) {
            auto neighbors = get_neighbors(p);
            std::size_t count_neighbors =
              std::count_if(neighbors.begin(), neighbors.end(),
                            [&flipped](const auto& p) { return flipped.contains(p); });

            // which black tiles stay black
            if (count_neighbors == 1 || count_neighbors == 2)
                next.insert(p);

            // which white tiles turn black
            for (const auto& n : neighbors) {
                if (!flipped.contains(n) && !next.contains(n)) {
                    auto n_neighbors = get_neighbors(n);
                    if (std::count_if(n_neighbors.begin(), n_neighbors.end(),
                                      [&flipped](const auto& p) { return flipped.contains(p); })
                        == 2)
                        next.insert(n);
                }
            }

            // all others default to white
        }
        flipped = std::move(next);
    }

    std::cout << flipped.size() << std::endl;

    return 0;
}

#endif // DAY_24
