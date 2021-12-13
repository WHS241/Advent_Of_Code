#ifndef DAY_16
#define DAY_16 2

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

int day_16_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    const std::unordered_map<std::string, std::size_t> input = {
      {"children", 3}, {"cats", 7},     {"samoyeds", 2}, {"pomeranians", 3}, {"akitas", 0},
      {"vizslas", 0},  {"goldfish", 5}, {"trees", 3},    {"cars", 2},        {"perfumes", 1}};

    const std::unordered_map<std::string, std::function<bool(std::size_t, std::size_t)>> comp = {
#if DAY_16 == 1
        {"cats", std::equal_to<std::size_t>()},
        {"trees", std::equal_to<std::size_t>()},
        {"pomeranians", std::equal_to<std::size_t>()},
        {"goldfish", std::equal_to<std::size_t>()},
#elif DAY_16 == 2
        {"cats", std::greater<std::size_t>()},
        {"trees", std::greater<std::size_t>()},
        {"pomeranians", std::less<std::size_t>()},
        {"goldfish", std::less<std::size_t>()},
#endif
        {"children", std::equal_to<std::size_t>()},
        {"samoyeds", std::equal_to<std::size_t>()},
        {"akitas", std::equal_to<std::size_t>()},
        {"vizslas", std::equal_to<std::size_t>()},
        {"cars", std::equal_to<std::size_t>()},
        {"perfumes", std::equal_to<std::size_t>()}
    };

    std::ifstream reader(argv[1]);

    std::vector<std::unordered_map<std::string, std::size_t>> knowns;

    for (std::string line; std::getline(reader, line);) {
        knowns.push_back(std::unordered_map<std::string, std::size_t>());
        std::istringstream line_read(line);

        std::size_t word_num = 0;
        std::string key;
        for (std::string word; line_read >> word;) {
            while (std::ispunct(word.back()))
                word.pop_back();
            if (word_num >= 2) {
                if (word_num % 2 == 0) {
                    key = word;
                } else {
                    knowns.back().emplace(key, std::stoul(word));
                }
            }
            ++word_num;
        }
    }

    for (std::size_t i = 0; i < knowns.size(); ++i)
        if (std::all_of(knowns[i].begin(), knowns[i].end(),
                        [&input, &comp](const std::pair<const std::string, std::size_t>& p) {
                            return input.find(p.first) == input.end()
                                || comp.at(p.first)(p.second, input.at(p.first));
                        })) {
            std::cout << i + 1 << std::endl;
        }

    return 0;
}

#endif
