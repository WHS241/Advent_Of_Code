#ifndef DAY_17
#define DAY_17 2

#include <iostream>
#include <list>

struct node {
    std::size_t x;
    node* n;
};

int day_17_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::size_t step_size       = std::stoul(argv[1]);
    constexpr std::size_t FINAL = DAY_17 == 1 ? 2017 : 50000000;

#if DAY_17 == 1
    node* n = new node;
    n->x    = 0;
    n->n    = n;

    for (std::size_t i = 1; i <= FINAL; ++i) {
        for (std::size_t j = 0; j < step_size % i; ++j)
            n = n->n;

        node* s = new node{i, n->n};
        n->n    = s;
        n       = s;
    }

    std::cout << n->n->x << std::endl;
#elif DAY_17 == 2
    std::size_t pos  = 0;
    std::size_t next = 0;

    for (std::size_t i = 1; i <= FINAL; ++i) {
        pos = (pos + step_size) % i;
        if (pos == 0)
            next = i;
        ++pos;
    }

    std::cout << next << std::endl;
#endif

    return 0;
}

#endif // DAY_17
