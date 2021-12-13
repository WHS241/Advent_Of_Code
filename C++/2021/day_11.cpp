#ifndef DAY_11
#define DAY_11

#include <fstream>
#include <iostream>
#include <vector>

int day_11_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_it = std::stoul(argv[2]);

    std::vector<std::vector<std::size_t>> energy;

    std::size_t part_1 = 0;
    std::size_t part_2 = 0;

    for (std::string line; std::getline(reader, line);) {
        energy.emplace_back();
        for (char c : line)
            energy.back().push_back(c - '0');
    }

    for (std::size_t i = 0; part_2 == 0 || i < num_it; ++i) {
        std::vector<std::vector<bool>> flashed(energy.size(),
                                               std::vector<bool>(energy.front().size(), false));
        std::vector<std::pair<std::size_t, std::size_t>> queue;

        for (std::size_t j = 0; j < energy.size(); ++j)
            for (std::size_t k = 0; k < energy.front().size(); ++k)
                queue.emplace_back(j, k);

        for (auto& r : energy)
            for (auto& v : r)
                ++v;

        while (!queue.empty()) {
            std::pair<std::size_t, std::size_t> curr = queue.back();
            queue.pop_back();
            if (!flashed[curr.first][curr.second] && energy[curr.first][curr.second] > 9) {
                flashed[curr.first][curr.second] = true;
                if (curr.first > 0) {
                    ++energy[curr.first - 1][curr.second];
                    queue.emplace_back(curr.first - 1, curr.second);
                    if (curr.second > 0) {
                        ++energy[curr.first - 1][curr.second - 1];
                        queue.emplace_back(curr.first - 1, curr.second - 1);
                    }
                    if (curr.second < energy.front().size() - 1) {
                        ++energy[curr.first - 1][curr.second + 1];
                        queue.emplace_back(curr.first - 1, curr.second + 1);
                    }
                }
                if (curr.first < energy.size() - 1) {
                    ++energy[curr.first + 1][curr.second];
                    queue.emplace_back(curr.first + 1, curr.second);
                    if (curr.second > 0) {
                        ++energy[curr.first + 1][curr.second - 1];
                        queue.emplace_back(curr.first + 1, curr.second - 1);
                    }
                    if (curr.second < energy.front().size() - 1) {
                        ++energy[curr.first + 1][curr.second + 1];
                        queue.emplace_back(curr.first + 1, curr.second + 1);
                    }
                }
                if (curr.second > 0) {
                    ++energy[curr.first][curr.second - 1];
                    queue.emplace_back(curr.first, curr.second - 1);
                }
                if (curr.second < energy.front().size() - 1) {
                    ++energy[curr.first][curr.second + 1];
                    queue.emplace_back(curr.first, curr.second + 1);
                }
            }
        }

        for (auto& r : energy) {
            for (auto& v : r) {
                if (v > 9) {
                    if (i < num_it)
                        ++part_1;
                    v = 0;
                }
            }
        }

        if (std::all_of(flashed.begin(), flashed.end(), [](const std::vector<bool>& r) {
                return std::all_of(r.begin(), r.end(), std::identity());
            }))
            part_2 = i + 1;
    }

    std::cout << part_1 << '\n' << part_2 << std::endl;

    return 0;
}

#endif // DAY_11
