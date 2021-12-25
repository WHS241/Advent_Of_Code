#ifndef DAY_25
#define DAY_25

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int day_25_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<std::string> herd(std::istream_iterator<std::string>{reader},
                                  std::istream_iterator<std::string>{});
    std::size_t num_steps = 0;
    bool gridlock = false;
    while (!gridlock) {
        std::vector<std::string> next(herd);
        gridlock = true;
        for (std::size_t i = 0; i < next.size(); ++i) {
            for (std::size_t j = 0; j < next.back().size(); ++j) {
                if (herd[i][j] == '>' && herd[i][(j + 1) % next.back().size()] == '.') {
                    gridlock = false;
                    next[i][(j + 1) % next.back().size()] = '>';
                    next[i][j] = '.';
                }
            }
        }

        herd = next;
        for (std::size_t i = 0; i < next.size(); ++i) {
            for (std::size_t j = 0; j < next.back().size(); ++j) {
                if (herd[i][j] == 'v' && herd[(i + 1) % next.size()][j] == '.') {
                    gridlock = false;
                    next[(i + 1) % next.size()][j] = 'v';
                    next[i][j] = '.';
                }
            }
        }
        herd = next;
        ++num_steps;
    }

    std::cout << num_steps << std::endl;

    return 0;
}

#endif // DAY_25
