#ifndef DAY_15
#define DAY_15 2

#include <iostream>

#if DAY_15 == 1
constexpr std::size_t NUM_ROUNDS = 40000000;
#elif DAY_15 == 2
constexpr std::size_t NUM_ROUNDS = 5000000;
#endif

class engine {
    private:
    std::size_t curr;
    std::size_t factor;
    std::size_t criteria;

    public:
    engine(std::size_t seed, std::size_t factor, std::size_t criteria = 1) :
        curr(seed), factor(factor), criteria(criteria) {}

    std::size_t operator()() {
        do {
            curr = (curr * factor) % 2147483647;
        } while (curr % criteria != 0);
        return curr;
    }
};

int day_15_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    constexpr std::size_t FACTOR_1 = 16807;
    constexpr std::size_t FACTOR_2 = 48271;

    constexpr std::size_t CRIT_1 = 4;
    constexpr std::size_t CRIT_2 = 8;

#if DAY_15 == 1
    engine e1(std::stoul(argv[1]), FACTOR_1), e2(std::stoul(argv[2]), FACTOR_2);
#elif DAY_15 == 2
    engine e1(std::stoul(argv[1]), FACTOR_1, CRIT_1), e2(std::stoul(argv[2]), FACTOR_2, CRIT_2);
#endif

    std::size_t count = 0;

    for (std::size_t i = 0; i < NUM_ROUNDS; ++i) {
        std::size_t n1 = e1();
        std::size_t n2 = e2();

        if (n1 % 0x10000 == n2 % 0x10000)
            ++count;
    }

    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_15
