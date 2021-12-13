#ifndef DAY_15
#define DAY_15

#include <iostream>
#include <unordered_map>

int day_15_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::size_t target = std::stoul(argv[1]);

    std::unordered_map<std::size_t, std::size_t> last;
    for (int i = 2; i < argc; ++i)
        last[std::stoul(argv[i])] = i - 1;

    std::size_t x = 0;
    std::size_t i = argc - 1;

    while (i != target) {
        std::size_t next = last.count(x) ? i - last[x] : 0;
        last[x] = i++;
        x = next;
    }

    std::cout << x << std::endl;

    return 0;
}

#endif // DAY_15
