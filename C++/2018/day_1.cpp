#ifndef DAY_1
#define DAY_1 2

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

#if DAY_1 == 1
    int total = 0;
#elif DAY_1 == 2
    std::unordered_set<int> history;
    std::vector<int> shifts;
#endif

    for (std::string line; std::getline(reader, line);)
#if DAY_1 == 1
        total += std::stoi(line);
#elif DAY_1 == 2
        shifts.push_back(std::stoi(line));
#endif

#if DAY_1 == 1
    std::cout << total << std::endl;
#elif DAY_1 == 2
    int curr = 0;
    history.insert(0);

    while (true) {
        for (int x : shifts) {
            curr += x;
            if (history.count(curr)) {
                std::cout << curr << std::endl;
                return 0;
            }
            history.insert(curr);
        }
    }
#endif

    return 0;
}

#endif // DAY_1
