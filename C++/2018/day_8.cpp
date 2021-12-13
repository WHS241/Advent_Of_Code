#ifndef DAY_8
#define DAY_8 2

#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

int day_8_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::size_t> data(std::istream_iterator<std::size_t>{reader},
                                  std::istream_iterator<std::size_t>{});

#if DAY_8 == 1
    std::vector<std::tuple<std::size_t, std::size_t, std::size_t>> node_stack;
    node_stack.emplace_back(data[0], data[1], 0);
#elif DAY_8 == 2
    std::vector<std::tuple<std::size_t, std::size_t, std::vector<std::size_t>>> node_stack;
    node_stack.emplace_back(data[0], data[1], std::vector<std::size_t>());
#endif

    std::size_t i = 2; // points to either front of next node or start of metadata

#if DAY_8 == 1
    std::size_t sum = 0;
#endif

    while (!node_stack.empty()) {
#if DAY_8 == 1
        std::size_t n, m, c;
        std::tie(n, m, c) = node_stack.back();

        if (n == c) {
            for (std::size_t j = 0; j < m; ++j)
                sum += data[i + j];
            i += m;
            node_stack.pop_back();
            if (!node_stack.empty())
                ++std::get<2>(node_stack.back());
        } else {
            std::size_t num_children = data[i++];
            std::size_t num_metadata = data[i++];
            node_stack.emplace_back(num_children, num_metadata, 0);
        }
#elif DAY_8 == 2
        auto [n, m, c] = node_stack.back();

        if (n == c.size()) {
            std::size_t sum = 0;
            for (std::size_t j = 0; j < m; ++i, ++j)
                sum += n == 0 ? data[i] : (data[i] <= n ? c[data[i] - 1] : 0);
            node_stack.pop_back();
            if (!node_stack.empty())
                std::get<2>(node_stack.back()).push_back(sum);
            else
                std::cout << sum << std::endl;
        } else {
            std::size_t num_children = data[i++];
            std::size_t num_metadata = data[i++];
            node_stack.emplace_back(num_children, num_metadata, std::vector<std::size_t>());
        }
#endif
    }

#if DAY_8 == 1
    std::cout << sum << std::endl;
#endif

    return 0;
}

#endif // DAY_8
