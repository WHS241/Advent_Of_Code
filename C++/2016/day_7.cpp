#ifndef DAY_7
#define DAY_7 2

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

bool is_within_hypernet(const std::string& ip, std::size_t i) {
    std::size_t next_close_brace = ip.find(']', i);
    return next_close_brace != std::string::npos && ip.find('[', i) > next_close_brace;
}

int day_7_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t count_p1(0), count_p2(0);

    for (std::string line; std::getline(reader, line);) {
        // Part 1
        bool supports_tls = false;
        for (std::size_t i = 0; i < line.size() - 3; ++i) {
            if (line[i] == line[i + 3] && line[i + 1] == line[i + 2] && line[i] != line[i + 1]) {
                if (is_within_hypernet(line, i)) {
                    supports_tls = false;
                    break;
                } else {
                    supports_tls = true;
                }
            }
        }

        if (supports_tls)
            ++count_p1;

        // Part 2
        std::unordered_map<char, std::pair<std::unordered_set<char>, std::unordered_set<char>>>
          three_seqs; // a -> { {c}, {b} } <=> "aca" outside hypernet, "aba" inside
        for (std::size_t i = 0; i < line.size() - 2; ++i) {
            if (line[i] == line[i + 2] && line[i] != line[i + 1] && line[i + 1] != '['
                && line[i + 1] != ']') {
                if (is_within_hypernet(line, i))
                    three_seqs[line[i]].second.insert(line[i + 1]);
                else
                    three_seqs[line[i]].first.insert(line[i + 1]);
            }
        }

        bool supports_ssl = false;
        for (auto it = three_seqs.begin(); !supports_ssl && it != three_seqs.end(); ++it) {
            for (auto it2 = it->second.first.begin();
                 !supports_ssl && it2 != it->second.first.end(); ++it2)
                if (three_seqs.count(*it2) && three_seqs[*it2].second.count(it->first))
                    supports_ssl = true;

            for (auto it2 = it->second.second.begin();
                 !supports_ssl && it2 != it->second.second.end(); ++it2)
                if (three_seqs.count(*it2) && three_seqs[*it2].first.count(it->first))
                    supports_ssl = true;
        }

        if (supports_ssl)
            ++count_p2;
    }

    std::cout << count_p1 << '\n' << count_p2 << std::endl;

    return 0;
}

#endif
