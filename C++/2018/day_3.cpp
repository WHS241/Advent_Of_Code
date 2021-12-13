#ifndef DAY_3
#define DAY_3 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>

struct request {
    std::size_t id;
    std::pair<std::size_t, std::size_t> x_lim;
    std::pair<std::size_t, std::size_t> y_lim;
};

request parse(const std::string& s) {
    request result;
    std::istringstream r(s);
    r.ignore(1);
    r >> result.id;
    r.ignore(3, '@');
    r >> result.x_lim.first;
    r.ignore(1);
    r >> result.y_lim.first;
    r.ignore(2, ' ');
    std::size_t dx, dy;
    r >> dx;
    r.ignore(1);
    r >> dy;

    result.x_lim.second = result.x_lim.first + dx;
    result.y_lim.second = result.y_lim.first + dy;

    return result;
}

int day_3_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    constexpr std::size_t GRID_X(1000), GRID_Y(1000);

    std::array<std::array<std::size_t, GRID_Y>, GRID_X> fabric;
    for (std::array<std::size_t, GRID_Y>& r : fabric)
        std::fill(r.begin(), r.end(), 0UL);

    std::vector<request> reqs;

    for (std::string line; std::getline(reader, line);) {
        request r = parse(line);
        for (std::size_t i = r.x_lim.first; i < r.x_lim.second; ++i)
            for (std::size_t j = r.y_lim.first; j < r.y_lim.second; ++j)
                ++fabric[i][j];

        reqs.push_back(r);
    }

#if DAY_3 == 1
    std::cout << std::transform_reduce(
      fabric.begin(), fabric.end(), 0UL, std::plus<>(),
      [](const std::array<std::size_t, GRID_Y>& r) {
          return std::count_if(r.begin(), r.end(), [](std::size_t x) { return x > 1; });
      }) << std::endl;
#elif DAY_3 == 2
    for (const request& r : reqs) {
        bool intact = true;
        for (std::size_t i = r.x_lim.first; intact && i < r.x_lim.second; ++i)
            for (std::size_t j = r.y_lim.first; intact && j < r.y_lim.second; ++j)
                if (fabric[i][j] != 1)
                    intact = false;

        if (intact)
            std::cout << r.id << std::endl;
    }
#endif

    return 0;
}

#endif // DAY_3
