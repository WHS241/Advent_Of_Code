#ifndef DAY_20
#define DAY_20

#include <fstream>
#include <iostream>
#include <map>

constexpr std::size_t RANGE = 4294967296UL;

int day_20_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::map<std::size_t, std::size_t> excluded;
    for (std::string line; std::getline(reader, line);) {
        std::size_t sep = line.find('-');
        std::size_t lower = std::stoul(line.substr(0, sep));
        std::size_t upper = std::stoul(line.substr(sep + 1));

        auto ins = excluded.insert({lower, upper});
        if (!ins.second) {
            if (ins.first->second < lower) {
                std::size_t temp = ins.first->second;
                excluded.erase(lower);
                ins.first = excluded.find(temp);
                ins.first->second = upper;
                lower = temp;
            } else if (ins.first->second < upper) {
                excluded.erase(ins.first->second);
                ins.first->second = upper;
            } else {
                continue;
            }
        } else if (ins.first != excluded.begin()) {
            auto temp = ins.first;
            --temp;
            if (temp->second > upper) {
                excluded.erase(lower);
                continue;
            } else if (temp->second > lower) {
                excluded.erase(lower);
                temp->second = upper;
                lower = temp->first;
                ins.first = temp;
            }
        }

        auto ins2 = excluded.insert({upper, lower});
        if (!ins2.second) {
            // ins2.first->second < lower not possible
            if (ins2.first->second > upper) {
                std::size_t temp = ins2.first->second;
                excluded.erase(upper);
                ins2.first = excluded.find(temp);
                ins2.first->second = lower;
                ins.first->second = temp;
            } else if (ins2.first->second > lower) {
                excluded.erase(ins2.first->second);
                ins2.first->second = lower;
            }
        } else {
            auto temp = ins2.first;
            ++temp;
            if (temp != excluded.end()) {
                if (temp->second < upper) {
                    excluded.erase(upper);
                    temp->second = lower;
                    upper = temp->first;
                    ins.first->second = upper;
                    ins2.first = temp;
                }
            }
        }

        auto it = ins.first;
        for (++it; it != ins2.first;)
            it = excluded.erase(it);
    }

    bool found_min = false;
    std::size_t i = 0;
    std::size_t count = 0;
    while (i < RANGE) {
        if (excluded.count(i)) {
            i = excluded[i];
        } else {
            ++count;
            if (!found_min) {
                std::cout << i << '\n';
                found_min = true;
            }
        }
        ++i;
    }
    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_20
