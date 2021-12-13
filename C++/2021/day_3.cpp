#ifndef DAY_3
#define DAY_3

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int day_3_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::string> numbers(std::istream_iterator<std::string>{reader},
                                     std::istream_iterator<std::string>{});
    std::vector<std::size_t> count_1s(numbers.front().size());

    for (const std::string& num : numbers)
        for (std::size_t i = 0; i < num.size(); ++i)
            if (num[i] == '1')
                ++count_1s[i];

    std::size_t delta(0), epsilon(0);

    std::list<std::string> pot_o2_gen(numbers.begin(), numbers.end()), pot_co2_scrub(pot_o2_gen);

    for (std::size_t i = 0; i < numbers.front().size(); ++i) {
        std::size_t c = count_1s[i];
        delta *= 2;
        epsilon *= 2;

        if (c < numbers.size() - c)
            ++epsilon;
        else
            ++delta;
    }

    for (std::size_t i = 0; pot_o2_gen.size() > 1; ++i) {
        std::size_t count = std::count_if(pot_o2_gen.begin(), pot_o2_gen.end(),
                                          [i](const std::string& s) { return s[i] == '1'; });
        char more_common = (count < pot_o2_gen.size() - count) ? '0' : '1';
        pot_o2_gen.remove_if([i, more_common](const auto& s) { return s[i] != more_common; });
    }

    for (std::size_t i = 0; pot_co2_scrub.size() > 1; ++i) {
        std::size_t count = std::count_if(pot_co2_scrub.begin(), pot_co2_scrub.end(),
                                          [i](const std::string& s) { return s[i] == '1'; });
        char more_common = (count < pot_co2_scrub.size() - count) ? '0' : '1';
        pot_co2_scrub.remove_if([i, more_common](const auto& s) { return s[i] == more_common; });
    }

    std::cout << delta * epsilon << '\n'
              << std::stoul(pot_o2_gen.front(), nullptr, 2)
                   * std::stoul(pot_co2_scrub.front(), nullptr, 2)
              << std::endl;

    return 0;
}

#endif // DAY_3
