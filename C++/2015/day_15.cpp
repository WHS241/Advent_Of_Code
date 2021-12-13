#ifndef DAY_15
#define DAY_15

#define DAY_15_PART 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <vector>

constexpr std::size_t TOTAL = 100;
constexpr std::size_t CALORIE_TOTAL = 500;

constexpr std::size_t NUM_PROPS = 5;
typedef std::array<int, NUM_PROPS> props;

std::size_t get_total(const std::vector<props>& p, const std::vector<std::size_t>& amounts) {
    std::array<int, NUM_PROPS> totals;
    std::fill(totals.begin(), totals.end(), 0);

    auto it = p.begin();
    auto it2 = amounts.begin();
    while (it != p.end()) {
        auto it3 = it->begin();
        auto it4 = totals.begin();
        while (it4 != totals.end())
            *(it4++) += *(it3++) * *it2;
        ++it;
        ++it2;
    }

#if DAY_15_PART == 2
    if (totals.back() != CALORIE_TOTAL)
        return 0;
#endif

    std::size_t product = 1;
    for (std::size_t i = 0; i < totals.size() - 1; ++i) {
        if (totals[i] <= 0)
            return 0;
        product *= totals[i];
    }
    return product;
}

bool advance(std::vector<std::size_t>& v) {
    std::size_t t_minus_last = std::accumulate(v.begin(), v.end() - 1, 0);
    for (auto it = v.rbegin() + 1; it != v.rend(); ++it) {
        t_minus_last -= *it;
        *it = (t_minus_last == TOTAL) ? 0 : ((*it + 1) % (TOTAL - t_minus_last + 1));
        if (*it != 0)
            break;
    }

    if (std::all_of(v.begin(), v.end() - 1, [](std::size_t x) { return x == 0; }))
        return false;

    v.back() = TOTAL - std::accumulate(v.begin(), v.end() - 1, 0);
    return true;
}

int day_15_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<props> input;

    for (std::string line; std::getline(reader, line);) {
        input.push_back(props());
        std::istringstream line_read(line);
        line_read.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        for (std::size_t i = 0; i < NUM_PROPS; ++i)
            line_read >> input.back().at(i);
    }

    std::size_t max_total = 0;
    std::vector<std::size_t> amounts(input.size(), 0);
    amounts.back() = TOTAL;

    do {
        max_total = std::max(max_total, get_total(input, amounts));
    } while (advance(amounts));

    std::cout << max_total << std::endl;
    return 0;
}

#endif // DAY_15
