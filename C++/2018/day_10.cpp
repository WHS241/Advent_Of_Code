#ifndef DAY_10
#define DAY_10

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <tuple>
#include <vector>

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::tuple<int, int, int, int>> particles;

    int minX = std::numeric_limits<int>::max(), maxX = std::numeric_limits<int>::min();
    int minY(minX), maxY(maxX);

    for (std::string line; std::getline(reader, line);) {
        std::istringstream lr(line);
        lr.ignore(std::numeric_limits<std::streamsize>::max(), '<');
        int px, py, vx, vy;
        lr >> px;
        lr.ignore(2, ',');
        lr >> py;
        lr.ignore(std::numeric_limits<std::streamsize>::max(), '<');
        lr >> vx;
        lr.ignore(2, ',');
        lr >> vy;

        minX = std::min(minX, px);
        minY = std::min(minY, py);
        maxX = std::max(maxX, px);
        maxY = std::max(maxY, py);

        particles.emplace_back(px, py, vx, vy);
    }

    bool scattering = false;
    std::size_t i = 0;

    while (!scattering) {
        auto copy(particles);
        int nminX, nmaxX, nminY, nmaxY;
        nminX = nminY = std::numeric_limits<int>::max();
        nmaxX = nmaxY = std::numeric_limits<int>::min();

        for (auto& p : copy) {
            auto [px, py, vx, vy] = p;
            px += vx;
            py += vy;
            p = {px, py, vx, vy};
            nminX = std::min(nminX, px);
            nmaxX = std::max(nmaxX, px);
            nminY = std::min(nminY, py);
            nmaxY = std::max(nmaxY, py);
        }

        scattering = nmaxX - nminX + nmaxY - nminY > maxX - minX + maxY - minY;
        if (!scattering) {
            maxX = nmaxX;
            minX = nminX;
            maxY = nmaxY;
            minY = nminY;
            particles = std::move(copy);
        } else
            ++i;
    }

    std::vector<std::vector<bool>> loc(maxY - minY + 1, std::vector<bool>(maxX - minX + 1, false));
    for (const auto& p : particles) {
        auto [px, py, vx, vy] = p;
        loc[py - minY][px - minX] = true;
    }

    for (std::size_t i = 0; i < loc.size(); ++i) {
        for (std::size_t j = 0; j < loc[0].size(); ++j)
            std::cout << (loc[i][j] ? "\033[47m \033[0m" : " ");

        std::cout << '\n';
    }

    std::cout << i << std::endl;

    return 0;
}

#endif // DAY_10
