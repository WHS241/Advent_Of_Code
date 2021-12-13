#ifndef DAY_25
#define DAY_25

#include <iostream>

std::size_t mod_pow(std::size_t b, std::size_t x, std::size_t mod) {
    if (x == 0)
        return 1;
    std::size_t h = mod_pow(b, x / 2, mod);
    h = (h * h) % mod;
    if (x % 2)
        h = (h * b) % mod;
    return h;
}

int day_25_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::size_t target1(std::stoul(argv[1])), target2(std::stoul(argv[2]));
    bool found1(false), found2(false);
    std::size_t count1(1), count2(1);

    constexpr std::size_t MOD = 20201227;
    constexpr std::size_t BASE = 7;

    std::size_t curr = BASE;
    while (!found1 || !found2) {
        if (curr == target1)
            found1 = true;
        else if (!found1)
            ++count1;

        if (curr == target2)
            found2 = true;
        else if (!found2)
            ++count2;

        curr = (curr * BASE) % MOD;
    }

    std::size_t key = mod_pow(BASE, count1, MOD);
    std::cout << mod_pow(key, count2, MOD) << std::endl;

    return 0;
}

#endif // DAY_25
