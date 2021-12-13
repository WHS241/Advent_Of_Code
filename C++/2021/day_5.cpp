#ifndef DAY_5
#define DAY_5

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_map<std::size_t, std::unordered_map<std::size_t, std::size_t>> vents_p1,
      vents_p2;

    for (std::string line; std::getline(reader, line);) {
        std::size_t x1, x2, y1, y2;
        std::istringstream parser(line);
        parser >> x1;
        parser.get();
        parser >> y1;
        parser.ignore(4, '>');
        parser >> x2;
        parser.get();
        parser >> y2;

        if (x1 == x2) {
            if (y1 >= y2)
                std::swap(y1, y2);
            for (std::size_t i = y1; i <= y2; ++i) {
                ++vents_p1[x1][i];
                ++vents_p2[x1][i];
            }
        } else if (y1 == y2) {
            if (x1 >= x2)
                std::swap(x1, x2);
            for (std::size_t i = x1; i <= x2; ++i) {
                ++vents_p1[i][y1];
                ++vents_p2[i][y1];
            }
        } else {
            if (x1 >= x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            if (y1 < y2)
                for (std::size_t i = 0; i <= x2 - x1; ++i)
                    ++vents_p2[x1 + i][y1 + i];
            else
                for (std::size_t i = 0; i <= x2 - x1; ++i)
                    ++vents_p2[x1 + i][y1 - i];
        }
    }

    std::cout << std::transform_reduce(
      vents_p1.begin(), vents_p1.end(), 0UL, std::plus<>(),
      [](const auto& p) {
          return std::count_if(p.second.begin(), p.second.end(),
                               [](const auto& p1) { return p1.second > 1; });
      }) << '\n'
              << std::transform_reduce(vents_p2.begin(), vents_p2.end(), 0UL, std::plus<>(),
                                       [](const auto& p) {
                                           return std::count_if(
                                             p.second.begin(), p.second.end(),
                                             [](const auto& p1) { return p1.second > 1; });
                                       })
              << std::endl;

    return 0;
}

#endif // DAY_5
