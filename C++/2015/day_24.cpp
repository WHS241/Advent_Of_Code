#ifndef DAY_24
#define DAY_24

#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int day_24_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_comparts = std::stoul(argv[2]);
    std::vector<std::size_t> packages;

    for (std::size_t weight; reader >> weight;)
        packages.push_back(weight);

    std::vector<short> group_num(packages.size());
    std::size_t total = std::accumulate(packages.begin(), packages.end(), 0UL);

    if (total % num_comparts != 0) {
        std::cout << "不可能" << std::endl;
        return 1;
    }

    bool found_group = false;
    std::size_t min_size = 0;
    std::size_t min_product;

    while (!found_group) {
        ++min_size;
        min_product = ULONG_MAX;
        std::vector<std::vector<std::size_t>::reverse_iterator> selected(min_size);
        auto it = packages.rbegin();
        for (std::size_t i = 0; i < min_size; ++i)
            selected[i] = it++;

        if (std::transform_reduce(
              selected.begin(), selected.end(), std::size_t(0), std::plus<>(),
              std::mem_fn(&std::vector<std::size_t>::reverse_iterator::operator*))
            < total / num_comparts)
            continue;

        while (selected.back() != packages.rend()) {
            if (std::transform_reduce(
                  selected.begin(), selected.end(), std::size_t(0), std::plus<>(),
                  std::mem_fn(&std::vector<std::size_t>::reverse_iterator::operator*))
                == total / num_comparts) {
                found_group = true;
                min_product =
                  std::min(min_product,
                           std::transform_reduce(
                             selected.begin(), selected.end(), 1UL, std::multiplies<>(),
                             std::mem_fn(&std::vector<std::size_t>::reverse_iterator::operator*)));
            }

            for (auto it = selected.rbegin(); it != selected.rend(); ++it) {
                ++(*it);
                if (packages.rend() - *it > it - selected.rbegin()) {
                    std::size_t dist = 1;
                    for (auto it2 = it; it2 != selected.rbegin();)
                        *(--it2) = *it + dist++;
                    break;
                }
            }
        }
    }

    std::cout << min_product << std::endl;

    return 0;
}

#endif // DAY_24
