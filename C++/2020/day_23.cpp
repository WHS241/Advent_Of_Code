#ifndef DAY_23
#define DAY_23 2

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int day_23_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::list<long> cups;
    for (char c : std::string(argv[1]))
        cups.push_back(c - '0');
#if DAY_23 == 2
    while (cups.size() != 1000000)
        cups.push_back(cups.size() + 1);
#endif

    std::vector<std::list<long>::iterator> ptr(cups.size());
    for (auto it = cups.begin(); it != cups.end(); ++it) {
        ptr[*it - 1] = it;
    }

    std::size_t num_it = std::stoul(argv[2]);

    // The game
    for (std::size_t i = 0; i < num_it; ++i) {
        // Pick up 3 cups
        auto it = std::next(cups.begin());
        auto it2 = it;
        std::advance(it2, 3);
        std::list<long> removed;
        removed.splice(removed.end(), cups, it, it2);

        // Find destination cup
        std::size_t target = cups.front() != 1 ? cups.front() - 1 : cups.size() + removed.size();
        while (std::find(removed.begin(), removed.end(), target) != removed.end())
            target = target != 1 ? target - 1 : cups.size() + removed.size();

        auto ins = ptr[target - 1];
        cups.splice(std::next(ins), removed);
        cups.splice(cups.end(), cups, cups.begin());
    }

    // Move cup 1 to end for easier processing
    cups.splice(cups.end(), cups, cups.begin(), ptr[0]);
    cups.pop_front();

#if DAY_23 == 1
    std::copy(cups.begin(), cups.end(), std::ostream_iterator<long>(std::cout, ""));
#elif DAY_23 == 2
    std::cout << *cups.begin() * *std::next(cups.begin()) << std::endl;
#endif
    std::cout << std::endl;

    return 0;
}

#endif // DAY_23
