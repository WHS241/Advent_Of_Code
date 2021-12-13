#ifndef DAY_12
#define DAY_12

#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

/*
 * PART 2: 从大约100开始就只移动，没有变化
 * 输出值跟输入值有线性关系
 * f(x) - f(500) = m * (x - 500)
 */

int day_12_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::string input;

    reader.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    reader.ignore(1);
    std::getline(reader, input);
    std::string ignore;
    std::getline(reader, ignore);

    std::unordered_map<std::string, char> translation;

    for (std::string line; std::getline(reader, line);) {
        std::string state, result;
        std::istringstream parser(line);
        parser >> state >> ignore >> result;
        translation[state] = result.front();
    }

    std::deque<char> pots(input.begin(), input.end());
    long offset = 0;

    std::array<char, 6> env;
    env.back() = '\0';

    std::unordered_map<std::string, std::size_t> encountered;
    encountered[input] = 0;

    std::size_t lifetimes = std::stoul(argv[2]);
    for (std::size_t i = 0; i < lifetimes; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            pots.push_front('.');
            pots.push_back('.');
            --offset;
        }

        std::deque<char> temp(pots);
        auto it2 = temp.begin() + 2;
        for (auto it = pots.begin() + 2; it != pots.end() - 2; ++it, ++it2) {
            std::copy_n(it - 2, 5, env.begin());
            std::string rep = env.data();
            *it2 = translation.count(rep) ? translation[rep] : '.';
        }

        while (!temp.empty() && temp.back() == '.')
            temp.pop_back();

        while (!temp.empty() && temp.front() == '.') {
            temp.pop_front();
            ++offset;
        }
        pots = std::move(temp);
    }

    long total = 0;
    for (long i = 0; i < pots.size(); ++i)
        if (pots[i] == '#')
            total += i + offset;

    std::cout << total << std::endl;

    return 0;
}

#endif // DAY_12
