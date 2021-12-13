#ifndef DAY_5
#define DAY_5 2

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    std::ifstream reader(argv[1]);

    std::vector<int> jumps(std::istream_iterator<int>{reader}, std::istream_iterator<int>{});

    std::size_t count = 0;
    for (int i = 0; i >= 0 && i < jumps.size(); ++count) {
        int temp = jumps[i];
        if (temp >= 3)
            --jumps[i];
        else
            ++jumps[i];
        i += temp;
    }

    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_5
