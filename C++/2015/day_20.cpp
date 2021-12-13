#ifndef DAY_20
#define DAY_20 2

#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

std::unordered_map<std::size_t, std::unordered_map<std::size_t, std::size_t>> factors;
std::unordered_map<std::size_t, std::size_t> products = {{1, 1}};

std::unordered_map<std::size_t, std::size_t> get_prime_factorization(std::size_t x) {
    std::size_t orig = x;
    std::unordered_map<std::size_t, std::size_t> subresult;
    for (std::size_t i = 2; i <= std::ceil(std::sqrt(x)) && !factors.count(x); ++i) {
        while (x % i == 0 && !factors.count(x)) {
            ++subresult[i];
            x /= i;
        }
    }

    if (factors.count(x))
        for (const std::pair<const std::size_t, std::size_t>& v : factors[x])
            subresult[v.first] += v.second;
    else if (x == orig) {
        subresult[x] = 1;
    }

    if (orig != 1)
        factors[orig] = subresult;
    return subresult;
}

std::size_t sum_factors(std::size_t x) {
    // x = (p_1)^(k_1) * (p_2)^(k_2) * ...
    // sum factors = [((p_1)^(k_1 - 1))((p_2)^(k_2 - 1))...]/[(p_1-1)(p_2-1)...]
    // use products map to calculate from a previously known factor
    if (!products.count(x)) {
        std::unordered_map<std::size_t, std::size_t> val = get_prime_factorization(x);
        auto it = val.begin();
        std::size_t power = std::pow(it->first, it->second);
        std::size_t sum_for_div = products[x / it->first];

        products[x] = sum_for_div * (it->first * power - 1) / (power - 1);
    }
    return products[x];
}

int day_20_main(int argc, char** argv) {
    if (argc < 3)
        return 1;
    std::size_t input = std::stoul(argv[1]);
    std::size_t num_presents = std::stoul(argv[2]);

    std::size_t part_1 = 2;
    while (sum_factors(part_1) * num_presents < input)
        ++part_1;
    std::cout << part_1 << std::endl;

    if (argc < 4)
        return 0;
    std::vector<std::size_t> presents(input / num_presents);
    std::size_t limit = std::stoul(argv[3]);
    std::size_t part_2 = presents.size();
    for (std::size_t i = 1; i <= presents.size(); ++i) {
        for (std::size_t j = i; j <= presents.size() && j <= part_2 && j / i <= limit; j += i) {
            presents[j - 1] += i * num_presents;
            if (presents[j - 1] >= input)
                part_2 = std::min(part_2, j);
        }
    }

    std::cout << part_2 << std::endl;

    return 0;
}

#endif
