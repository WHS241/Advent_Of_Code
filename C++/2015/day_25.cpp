#ifndef DAY_25
#define DAY_25

#include <iostream>
#include <list>

int day_25_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    constexpr std::size_t mod = 33554393;
    constexpr std::size_t coeff = 20151125;
    constexpr std::size_t base = 252533;

    std::size_t row(std::stoul(argv[1])), col(std::stoul(argv[2]));
    std::size_t diag = row + col - 1;
    std::size_t num_full_its = (diag - 1) * diag / 2; // num entries in the full triangle
    std::size_t exp = num_full_its + col - 1;         // num entries to process in final row;

    std::list<bool> mult;
    while (exp != 0) {
        mult.push_front(exp % 2);
        exp /= 2;
    }

    std::size_t curr = 1;
    for (bool b : mult) {
        curr = (curr * curr) % mod;
        if (b)
            curr = (curr * base) % mod;
    }

    std::cout << (coeff * curr) % mod << std::endl;

    return 0;
}

#endif // DAY_25
