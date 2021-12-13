#ifndef DAY_4
#define DAY_4 2

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <unordered_map>
#include <vector>

int day_4_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::vector<std::string> rooms;
    std::vector<std::size_t> ids;

    for (std::string line; std::getline(reader, line);) {
        std::unordered_map<char, std::size_t> counts;

        std::size_t i = 0;
        for (; !std::isdigit(line[i]); ++i)
            if (std::isalpha(line[i]))
                ++counts[line[i]];

        std::map<std::size_t, std::set<char>, std::greater<std::size_t>> by_count;
        for (const std::pair<const char, std::size_t>& p : counts)
            by_count[p.second].insert(p.first);

        auto it = by_count.begin();
        auto it2 = it->second.begin();

        std::string expected_checksum;
        while (expected_checksum.size() < 5) {
            if (it2 != it->second.end()) {
                expected_checksum.push_back(*(it2++));
            } else {
                ++it;
                it2 = it->second.begin();
            }
        }

        std::size_t checksum_pos = line.find('[', i);
        if (expected_checksum == line.substr(checksum_pos + 1, 5)) {
            std::size_t sector_id = std::stoul(line.substr(i, checksum_pos - i));
            std::transform(line.begin(), line.begin() + checksum_pos, line.begin(),
                           [&sector_id](char c) -> char {
                               if (std::isdigit(c))
                                   return c;
                               if (std::isalpha(c))
                                   return (c - 'a' + sector_id) % 26 + 'a';

                               return ' ';
                           });
            rooms.push_back(line);
            ids.push_back(sector_id);
        }
    }

    std::copy(rooms.begin(), rooms.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << "Part 1: " << std::reduce(ids.begin(), ids.end()) << std::endl;

    return 0;
}

#endif // DAY_4
