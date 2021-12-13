#ifndef DAY_4
#define DAY_4 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>

int day_4_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::set<std::string> data;

    for (std::string line; std::getline(reader, line);)
        data.insert(line);

    std::size_t curr;
    std::unordered_map<std::size_t,
                       std::map<std::size_t, std::map<std::size_t, std::map<std::size_t, bool>>>>
      times; // times[guard][(mo, day, min)] = asleep

    for (const std::string& s : data) {
        constexpr std::size_t NUM_DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        std::size_t month = std::stoul(s.substr(6, 2));
        std::size_t day = std::stoul(s.substr(9, 2));
        std::size_t hr = std::stoul(s.substr(12, 2));
        std::size_t min = std::stoul(s.substr(15, 2));
        if (hr == 23) {
            ++day;
            hr = 0;
            min = 0;
        }
        if (day > NUM_DAYS[month - 1]) {
            ++month;
            day = 0;
        }

        std::string info = s.substr(19);
        if (info.front() == 'G') {
            curr = std::stoul(info.substr(7, info.find(' ', 7) - 7));
        } else if (hr == 0) {
            times[curr][month][day][min] = info.front() == 'f';
        }
    }

    std::unordered_map<std::size_t,
                       std::map<std::size_t, std::map<std::size_t, std::array<bool, 60>>>>
      expanded;
    for (const auto& pid : times) {
        for (const auto& mid : pid.second) {
            for (const auto& did : mid.second) {
                bool curr = false;
                std::size_t c_m = 0;
                for (const auto& t : did.second) {
                    while (c_m < t.first)
                        expanded[pid.first][mid.first][did.first][c_m++] = curr;
                    curr = t.second;
                }
                while (c_m < 60)
                    expanded[pid.first][mid.first][did.first][c_m++] = curr;
            }
        }
    }

#if DAY_4 == 1
    std::unordered_map<std::size_t, std::size_t> total;
    for (const auto& pid : expanded)
        for (const auto& mid : pid.second)
            for (const auto& did : mid.second)
                for (bool sleep : did.second)
                    if (sleep)
                        ++total[pid.first];

    std::size_t target = std::max_element(total.begin(), total.end(), [](auto p1, auto p2) {
                             return p1.second < p2.second;
                         })->first;

    std::size_t min, count(0);
    for (std::size_t i = 0; i < 60; ++i) {
        std::size_t curr_c(0);
        for (const auto& mid : expanded[target])
            for (const auto& did : mid.second)
                if (did.second[i])
                    ++curr_c;

        if (curr_c > count) {
            count = curr_c;
            min = i;
        }
    }

    std::cout << target * min << std::endl;
#elif DAY_4 == 2
    std::size_t min, id, count(0);
    for (std::size_t i = 0; i < 60; ++i) {
        std::unordered_map<std::size_t, std::size_t> times;
        for (const auto& pid : expanded)
            for (const auto& mid : pid.second)
                for (const auto& did : mid.second)
                    if (did.second[i])
                        ++times[pid.first];

        if (times.empty())
            continue;

        std::pair<std::size_t, std::size_t> cand = *std::max_element(
          times.begin(), times.end(), [](auto p1, auto p2) { return p1.second < p2.second; });

        if (cand.second > count) {
            std::tie(id, count) = cand;
            min = i;
        }
    }

    std::cout << min * id << std::endl;
#endif

    return 0;
}

#endif // DAY_4
