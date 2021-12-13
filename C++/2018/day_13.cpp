#ifndef DAY_13
#define DAY_13 2

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

constexpr char VERT('|'), HOR('-'), RD('/'), LD('\\'), XING('+');
constexpr char LC('<'), RC('>'), UC('^'), DC('v');

int day_13_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::map<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>>
      trains; // trains[pos] = {direction, next_turn}
    std::vector<std::string> railmap;

    std::size_t i = 0;
    for (std::string line; std::getline(reader, line); ++i) {
        for (std::size_t j = 0; j < line.size(); ++j) {
            if (line[j] == DC) {
                trains.emplace(std::make_pair(i, j), std::make_pair(0, 1));
                line[j] = HOR;
            } else if (line[j] == RC) {
                trains.emplace(std::make_pair(i, j), std::make_pair(1, 1));
                line[j] = VERT;
            } else if (line[j] == UC) {
                trains.emplace(std::make_pair(i, j), std::make_pair(2, 1));
                line[j] = HOR;
            } else if (line[j] == LC) {
                trains.emplace(std::make_pair(i, j), std::make_pair(3, 1));
            }
        }

        railmap.push_back(line);
    }

#if DAY_13 == 1
    bool crashed = false;

    while (!crashed)
#elif DAY_13 == 2
    while (trains.size() > 1)
#endif
    {
        auto copy(trains);

        for (std::pair<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>> x :
             trains) {
            if (!copy.count(x.first))
                continue;

            copy.erase(x.first);

            switch (x.second.first) {
            case 0:
                ++x.first.first;
                break;

            case 1:
                ++x.first.second;
                break;

            case 2:
                --x.first.first;
                break;

            case 3:
                --x.first.second;
                break;

            default:
                break;
            }

            switch (railmap[x.first.first][x.first.second]) {
            case LD:
                {
                    constexpr std::size_t NEW_DIR[4] = {1, 0, 3, 2};
                    x.second.first = NEW_DIR[x.second.first];
                }
                break;

            case RD:
                {
                    constexpr std::size_t NEW_DIR[4] = {3, 2, 1, 0};
                    x.second.first = NEW_DIR[x.second.first];
                }
                break;

            case XING:
                x.second.first = (x.second.first + x.second.second) % 4;
                x.second.second = (((x.second.second + 1) % 4 + 2) % 3 + 3) % 4;
                break;

            default:
                break;
            }

            if (copy.count(x.first)) {
#if DAY_13 == 1
                std::cout << x.first.second << ',' << x.first.first << std::endl;
                crashed = true;
                break;
#elif DAY_13 == 2
                copy.erase(x.first);
#endif
            } else {
                copy.insert(x);
            }
        }

        trains = std::move(copy);
    }

#if DAY_13 == 2
    auto p = trains.begin()->first;
    std::cout << p.second << ',' << p.first << std::endl;
#endif

    return 0;
}

#endif // DAY_13
