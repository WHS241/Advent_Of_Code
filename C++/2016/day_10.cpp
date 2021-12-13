#ifndef DAY_10
#define DAY_10

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::unordered_map<std::size_t,
                       std::pair<std::pair<std::size_t, bool>, std::pair<std::size_t, bool>>>
      gives;
    std::unordered_map<std::size_t, std::vector<std::size_t>> has;
    std::map<std::size_t, std::size_t> output;
    std::list<std::size_t> needs_to_compare;

    std::ifstream reader(argv[1]);
    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);
        std::string word;
        line_read >> word;
        if (word == "value") {
            line_read >> word;
            std::size_t value = std::stoul(word);
            line_read >> word >> word >> word >> word;
            std::size_t bot = std::stoul(word);
            has[bot].push_back(value);
            if (has[bot].size() > 1)
                needs_to_compare.push_back(bot);
        } else {
            line_read >> word;
            std::size_t from = std::stoul(word);
            line_read >> word >> word >> word >> word;
            bool is_final = word == "output";
            line_read >> word;
            std::size_t low_target = std::stoul(word);
            gives[from].first = {low_target, is_final};
            line_read >> word >> word >> word >> word;
            is_final = word == "output";
            line_read >> word;
            std::size_t high_target = std::stoul(word);
            gives[from].second = {high_target, is_final};
        }
    }

    while (!needs_to_compare.empty()) {
        std::size_t curr = needs_to_compare.front();
        needs_to_compare.pop_front();
        std::size_t min_value = has[curr].front(), max_value = has[curr].back();
        has[curr].clear();
        if (max_value < min_value)
            std::swap(min_value, max_value);
        std::cout << "机器人" << curr << "比" << min_value << "和" << max_value << '\n';
        auto next = gives[curr];
        if (next.first.second)
            output[next.first.first] = min_value;
        else {
            has[next.first.first].push_back(min_value);
            if (has[next.first.first].size() > 1)
                needs_to_compare.push_back(next.first.first);
        }
        if (next.second.second)
            output[next.second.first] = max_value;
        else {
            has[next.second.first].push_back(max_value);
            if (has[next.second.first].size() > 1)
                needs_to_compare.push_back(next.second.first);
        }
    }

    for (auto p : output)
        std::cout << "输出" << p.first << "：" << p.second << std::endl;

    return 0;
}

#endif // DAY_10
