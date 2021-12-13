#ifndef DAY_16
#define DAY_16 2

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

int day_16_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::vector<std::size_t>> tickets;
    std::vector<std::array<std::pair<std::size_t, std::size_t>, 2>> ranges;
    std::string line;
    std::vector<std::string> targets;

    // Read in ranges of valid values
    std::size_t i = 0;
    while (std::getline(reader, line) && !line.empty()) {
        targets.push_back(line.substr(0, line.find(':')));

        std::istringstream line_read(line);
        line_read.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        std::array<std::pair<std::size_t, std::size_t>, 2> entry;
        line_read >> entry[0].first;
        line_read.ignore(1);
        line_read >> entry[0].second;
        line_read.ignore(3);
        line_read >> entry[1].first;
        line_read.ignore(1);
        line_read >> entry[1].second;
        ranges.push_back(entry);
        ++i;
    }

    std::vector<std::vector<bool>> potential(ranges.size(), std::vector<bool>(ranges.size(), true));
    std::vector<std::size_t> matches(ranges.size(), ranges.size());
    std::vector<std::size_t> m(ranges.size(), 0);
    std::vector<std::size_t> my_ticket;

    std::size_t rate_invalid = 0; // Part 1

    while (std::getline(reader, line)) {
        if (line.empty() || !std::isdigit(line.front()))
            continue;

        std::regex re("(\\d+)");

        if (my_ticket.empty()) {
            std::transform(std::regex_iterator<std::string::iterator>(line.begin(), line.end(), re),
                           std::regex_iterator<std::string::iterator>(),
                           std::back_inserter(my_ticket),
                           [](auto s) { return std::stoul(s.str()); });
            continue;
        }

        std::vector<std::vector<bool>> curr_pot(potential);
        std::vector<std::size_t> curr_match(
          matches); // number of fields that can be matched to a pos
        bool valid = true;

        std::vector<std::size_t> values;
        std::transform(std::regex_iterator<std::string::iterator>(line.begin(), line.end(), re),
                       std::regex_iterator<std::string::iterator>(), std::back_inserter(values),
                       [](auto s) { return std::stoul(s.str()); });
        for (std::size_t i = 0; i < values.size(); ++i) {
            std::size_t val = values[i];

            // Tickets with invalid values
            if (std::all_of(ranges.begin(), ranges.end(),
                            [&val](const std::array<std::pair<std::size_t, std::size_t>, 2>& r) {
                                return std::all_of(
                                  r.begin(), r.end(),
                                  [&val](const std::pair<std::size_t, std::size_t>& p) {
                                      return val < p.first || val > p.second;
                                  });
                            })) {
                rate_invalid += val;
                valid = false;
            }

            // Part 2
            // Determine what fields are match candidates based on current ticket
            for (std::size_t j = 0; j < curr_pot[i].size(); ++j) {
                if (curr_pot[i][j]
                    && std::all_of(ranges[j].begin(), ranges[j].end(),
                                   [&val](const std::pair<std::size_t, std::size_t>& p) {
                                       return val < p.first || val > p.second;
                                   })) {
                    curr_pot[i][j] = false;
                    --curr_match[i];
                }
            }

            if (curr_match[i] == 0) {
                valid = false;
            }

            // Force as much as possible
            if (curr_match[i] == 1) {
                std::vector<std::size_t> to_assign;
                to_assign.push_back(i);

                while (!to_assign.empty()) {
                    std::size_t k = to_assign.back();
                    to_assign.pop_back();
                    std::size_t pos =
                      std::find(curr_pot[k].begin(), curr_pot[k].end(), true) - curr_pot[k].begin();
                    m[pos] = k;
                    for (std::size_t j = 0; j < curr_pot.size(); ++j)
                        if (j != k) {
                            std::size_t curr_c = curr_match[j];
                            if (curr_pot[j][pos])
                                --curr_match[j];
                            curr_pot[j][pos] = false;
                            if (curr_c != curr_match[j] && curr_match[j] == 1)
                                to_assign.push_back(j);
                        }
                }
            }
        }

        if (valid) {
            potential = std::move(curr_pot);
            matches = std::move(curr_match);
        }
    }

    std::cout << rate_invalid << '\n';
    std::size_t product = 1;
    for (std::size_t i = 0; i < 6; ++i) {
        product *= my_ticket[m[i]];
    }

    std::cout << product << std::endl;

    return 0;
}

#endif // DAY_16
