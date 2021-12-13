#ifndef DAY_5
#define DAY_5

#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>

void collapse(std::list<char>& polymer) {
    for (auto it = polymer.begin(); it != polymer.end();) {
        if (it == polymer.begin()) {
            ++it;
            continue;
        }

        auto it2 = it;
        --it2;
        if ((std::isupper(*it) && *it2 == std::tolower(*it))
            || (std::islower(*it) && *it2 == std::toupper(*it))) {
            polymer.erase(it2);
            it = polymer.erase(it);
        } else
            ++it;
    }
}

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::list<char> units(std::istreambuf_iterator<char>{reader}, std::istreambuf_iterator<char>{});

    if (units.back() == '\n')
        units.pop_back();

    collapse(units);

    std::cout << units.size() << std::endl;

    std::size_t min = std::numeric_limits<std::size_t>::max();
    for (char c = 'a'; c <= 'z'; ++c) {
        std::list temp(units);
        temp.remove_if([&c](char x) { return x == c || std::tolower(x) == c; });
        collapse(temp);
        min = std::min(min, temp.size());
    }

    std::cout << min << std::endl;

    return 0;
}

#endif // DAY_5
