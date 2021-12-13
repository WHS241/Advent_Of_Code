#ifndef DAY_16
#define DAY_16 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <vector>

struct string_hash {
    std::size_t operator()(const std::vector<std::size_t>& v) const {
        static std::hash<std::string> hasher;
        std::string rep;
        std::copy(v.begin(), v.end(), std::back_inserter(rep));
        return hasher(rep);
    }
};

int day_16_main(int argc, char** argv) {
#if DAY_16 == 1
    if (argc < 3)
#elif DAY_16 == 2
    if (argc < 4)
#endif
        return 1;
    std::size_t num_programs = std::stoul(argv[2]);

    std::vector<std::size_t> programs(num_programs);
    std::iota(programs.begin(), programs.end(), 0UL);
    std::vector<std::size_t> lookup(programs);

#if DAY_16 == 1
    std::size_t num_it = 1;
#elif DAY_16 == 2
    std::size_t num_it = std::stoul(argv[3]);
#endif

    std::unordered_set<std::vector<std::size_t>, string_hash> encountered;
    encountered.insert(programs);

    std::vector<std::vector<std::size_t>> steps;
    steps.push_back(programs);

    for (std::size_t i = 0; i < num_it; ++i) {
        std::ifstream reader(argv[1]);
        for (std::string line; std::getline(reader, line, ',');) {
            if (line[0] == 's') {
                std::size_t spin_amount = std::stoul(line.substr(1)) % num_programs;
                std::rotate(programs.rbegin(), programs.rbegin() + spin_amount, programs.rend());
                for (std::size_t i = 0; i < num_programs; ++i)
                    lookup[programs[i]] = i;
            } else {
                std::size_t sep = line.find('/');

                if (line[0] == 'x') {
                    std::size_t x1(std::stoul(line.substr(1, sep - 1))),
                      x2(std::stoul(line.substr(sep + 1)));
                    std::swap(programs[x1], programs[x2]);
                    std::swap(lookup[programs[x1]], lookup[programs[x2]]);
                } else if (line[0] == 'p') {
                    std::size_t p1(line[sep - 1] - 'a'), p2(line[sep + 1] - 'a');
                    std::swap(lookup[p1], lookup[p2]);
                    std::swap(programs[lookup[p1]], programs[lookup[p2]]);
                }
            }
        }

        if (encountered.count(programs)) {
            programs = steps[num_it % steps.size()];
            break;
        } else {
            encountered.insert(programs);
            steps.push_back(programs);
        }
    }

    std::transform(programs.begin(), programs.end(), std::ostreambuf_iterator<char>(std::cout),
                   [](std::size_t i) -> char { return i + 'a'; });
    std::cout << std::endl;

    return 0;
}

#endif // DAY_16
