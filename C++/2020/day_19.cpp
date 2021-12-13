#ifndef DAY_19
#define DAY_19

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include <util/pair_hash.h>

std::size_t
  obeys(const std::unordered_map<std::size_t, std::list<std::list<std::size_t>>>& dep_rules,
        const std::unordered_map<std::size_t, char>& base_rules, const std::string& s,
        std::size_t id, std::size_t index) {
    auto& npos = std::string::npos;
    if (!dep_rules.count(id) && !base_rules.count(id))
        return npos;
    if (base_rules.count(id))
        return s[index] == base_rules.at(id) ? index + 1 : npos;

    for (auto& choice : dep_rules.at(id)) {
        std::size_t matches = index;
        for (auto sub : choice) {
            matches = obeys(dep_rules, base_rules, s, sub, matches);
            if (matches == npos) {
                break;
            }
        }

        if (matches != npos)
            return matches;
    }

    return npos;
}

void to_CNF(std::unordered_map<std::size_t, std::list<std::list<std::size_t>>>& non_term,
            std::unordered_map<std::size_t, std::unordered_set<char>>& term) {
    // expand >2
    std::size_t unused = 0;
    std::unordered_set<std::size_t> keys;
    std::list<std::unordered_map<std::size_t, std::list<std::list<std::size_t>>>::value_type> tmp(
      non_term.begin(), non_term.end());
    for (auto& p : non_term)
        keys.insert(p.first);
    for (auto& p : term)
        keys.insert(p.first);
    for (auto& p : tmp) {
        for (auto& l : p.second) {
            if (l.size() > 2) {
                while (keys.contains(unused))
                    ++unused;
                keys.insert(unused);

                tmp.emplace_back(unused,
                                 std::list<std::list<std::size_t>>{std::list<std::size_t>()});
                tmp.back().second.front().splice(tmp.back().second.front().end(), l);
                l.splice(l.end(), tmp.back().second.front(), tmp.back().second.front().begin());
                l.push_back(unused);
            }
        }
    }
    non_term =
      std::unordered_map<std::size_t, std::list<std::list<std::size_t>>>(tmp.begin(), tmp.end());

    // remove unit
    for (auto& p : non_term) {
        for (auto it = p.second.begin(); it != p.second.end();) {
            if (it->size() == 1) {
                if (term.contains(it->front()))
                    for (char c : term[it->front()])
                        term[p.first].insert(c);
                if (non_term.contains(it->front()))
                    std::copy(non_term[it->front()].begin(), non_term[it->front()].end(),
                              std::back_inserter(p.second));
                it = p.second.erase(it);
            } else {
                ++it;
            }
        }
    }
}

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_map<std::size_t, std::list<std::list<std::size_t>>> rules;
    std::unordered_map<std::size_t, std::unordered_set<char>> char_rules;

    for (std::string line; std::getline(reader, line) && !line.empty();) {
        std::size_t id = std::stoul(line.substr(0, line.find(':')));

        std::istringstream parser(line);
        parser.ignore(std::numeric_limits<std::streamsize>::max(), ':');

        for (std::string part; parser >> part;) {
            if (part.front() == '"')
                char_rules[id].insert(part[1]);
            else {
                if (rules[id].empty() || part.front() == '|') {
                    rules[id].emplace_back();
                }

                if (std::isdigit(part.front()))
                    rules[id].back().push_back(std::stoul(part));
            }
        }
    }

    to_CNF(rules, char_rules);

    std::size_t count = std::count_if(
      std::istream_iterator<std::string>(reader), std::istream_iterator<std::string>(),
      [&rules, &char_rules](const std::string s) {
          // CYK: matches[start_pos][end_pos][rule]
          std::vector<std::vector<std::unordered_set<std::size_t>>> matches(
            s.size(), std::vector<std::unordered_set<std::size_t>>(s.size()));
          for (std::size_t i = 0; i < s.size(); ++i)
              for (const auto& p : char_rules)
                  if (p.second.contains(s[i]))
                      matches[i][i].insert(p.first);

          for (std::size_t i = 0; i <= s.size(); ++i)
              for (std::size_t j = 0; j < s.size() - i; ++j)
                  for (const auto& p : rules)
                      for (const auto& l : p.second) {
                          if (l.size() == 1) {
                              if (matches[j][i + j].contains(l.front()))
                                  matches[j][i + j].insert(p.first);
                          } else {
                              for (std::size_t k = j; k < i + j; ++k)
                                  if (matches[j][k].contains(l.front())
                                      && matches[k + 1][i + j].contains(l.back())) {
                                      matches[j][j + i].insert(p.first);
                                      break;
                                  }
                          }
                          if (matches[j][j + i].contains(p.first))
                              break;
                      }

          return matches.front().back().contains(0);
          /*
if DAY_19 == 1
              return obeys(rules, char_rules, s, 0UL, 0UL) == s.size();
elif DAY_19 == 2
// rule 0 is always 8 11, which can be translated to (series of 42) (series of 31)
// with constraint # 31's > 1 and # 42's > #31's
std::vector<std::size_t> first_type_indices;
first_type_indices.push_back(0);
while (first_type_indices.back() != std::string::npos)
first_type_indices.push_back(obeys(rules, char_rules, s, 42, first_type_indices.back()));

first_type_indices.pop_back();
bool found_pivot = false;

while (!found_pivot && !first_type_indices.empty()) {
std::size_t curr = first_type_indices.back();
first_type_indices.pop_back();

std::size_t num_31 = 0;
while (curr != std::string::npos && curr != s.size()) {
curr = obeys(rules, char_rules, s, 31, curr);
++num_31;
}

if (curr == s.size() && num_31 != 0 && num_31 < first_type_indices.size())
return true;
}

return false;
endif
*/
      });

    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_19
