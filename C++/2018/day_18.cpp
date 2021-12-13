#ifndef DAY_18
#define DAY_18

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

int day_18_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> yard;
    for (std::string line; std::getline(reader, line);) {
        yard.push_back(line);
    }

    std::map<std::vector<std::string>, std::size_t> history;

    std::size_t num_it = std::stoul(argv[2]);
    for (std::size_t i = 0; i < num_it; ++i) {
        if (history.contains(yard)) {
            std::size_t last = history[yard];
            yard = std::find_if(history.begin(), history.end(), [&num_it, &i, &last](auto p) {
                       return p.second == (num_it - last) % (i - last) + last;
                   })->first;
            break;
        }
        history[yard] = i;
        auto copy(yard);
        for (std::size_t j = 0; j < yard.size(); ++j) {
            for (std::size_t k = 0; k < yard[0].size(); ++k) {
                std::size_t num_trees(0), num_logs(0);
                for (std::size_t a = 0; a < 3; ++a) {
                    for (std::size_t b = 0; b < 3; ++b) {
                        if ((a != 1 || b != 1) && j + a - 1 < yard.size()
                            && k + b - 1 < yard[0].size()) {
                            if (yard[j + a - 1][k + b - 1] == '|')
                                ++num_trees;
                            else if (yard[j + a - 1][k + b - 1] == '#')
                                ++num_logs;
                        }
                    }
                }

                if (yard[j][k] == '.' && num_trees >= 3)
                    copy[j][k] = '|';
                else if (yard[j][k] == '|' && num_logs >= 3)
                    copy[j][k] = '#';
                else if (yard[j][k] == '#' && (num_trees == 0 || num_logs == 0))
                    copy[j][k] = '.';
            }
        }

        yard = std::move(copy);
    }

    std::size_t trees(0), logs(0);
    for (std::size_t i = 0; i < yard.size(); ++i)
        for (std::size_t j = 0; j < yard[0].size(); ++j)
            if (yard[i][j] == '|')
                ++trees;
            else if (yard[i][j] == '#')
                ++logs;

    std::cout << trees << '*' << logs << '=' << trees * logs << std::endl;

    return 0;
}

#endif // DAY_18
