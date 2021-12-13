#ifndef DAY_4
#define DAY_4

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool check_byr(const std::string& s) {
    if (s.size() != 4 || !std::all_of(s.begin(), s.end(), [](char c) { return std::isdigit(c); }))
        return false;

    std::size_t year = std::stoul(s);
    return year >= 1920 && year <= 2002;
}

bool check_iyr(const std::string& s) {
    if (s.size() != 4 || !std::all_of(s.begin(), s.end(), [](char c) { return std::isdigit(c); }))
        return false;

    std::size_t year = std::stoul(s);
    return year >= 2010 && year <= 2020;
}

bool check_eyr(const std::string& s) {
    if (s.size() != 4 || !std::all_of(s.begin(), s.end(), [](char c) { return std::isdigit(c); }))
        return false;

    std::size_t year = std::stoul(s);
    return year >= 2020 && year <= 2030;
}

bool check_hgt(const std::string& s) {
    try {
        std::size_t num = std::stoul(s.substr(0, s.size() - 2));
        std::string unit = s.substr(s.size() - 2);

        return (unit == "cm" && num >= 150 && num <= 193)
            || (unit == "in" && num >= 59 && num <= 76);
    } catch (...) { return false; }
}

bool check_hcl(const std::string& s) {
    return s.size() == 7 && s.front() == '#' && std::all_of(s.begin() + 1, s.end(), [](char c) {
               return std::isxdigit(c) && (std::isdigit(c) || std::islower(c));
           });
}

bool check_ecl(const std::string& s) {
    static const std::unordered_set<std::string> accepted{"amb", "blu", "brn", "gry",
                                                          "grn", "hzl", "oth"};
    return accepted.count(s);
}

bool check_pid(const std::string& s) {
    return s.size() == 9 && std::all_of(s.begin(), s.end(), [](char c) { return std::isdigit(c); });
}

int day_4_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::unordered_map<std::string, std::string>> passports;
    passports.emplace_back();

    for (std::string line; std::getline(reader, line);) {
        if (line.empty())
            passports.emplace_back();
        else {
            std::istringstream line_read(line);

            for (std::string word; line_read >> word;) {
                std::size_t pos = word.find(':');
                passports.back().emplace(word.substr(0, pos), word.substr(pos + 1));
            }
        }
    }

    std::unordered_map<std::string, std::function<bool(const std::string&)>> expected{
      {"byr", check_byr}, {"iyr", check_iyr}, {"eyr", check_eyr}, {"hgt", check_hgt},
      {"hcl", check_hcl}, {"ecl", check_ecl}, {"pid", check_pid}};

    auto p1_check = [&expected](const std::unordered_map<std::string, std::string>& fields) {
        return std::all_of(
          expected.begin(), expected.end(),
          [&fields](const std::pair<std::string, std::function<bool(const std::string&)>>& p)
            -> bool { return fields.count(p.first); });
    };
    auto p2_check = [&expected](const std::unordered_map<std::string, std::string>& fields) {
        return std::all_of(
          expected.begin(), expected.end(),
          [&fields](const std::pair<std::string, std::function<bool(const std::string&)>>& p)
            -> bool { return fields.count(p.first) && p.second(fields.at(p.first)); });
    };
    std::cout << std::count_if(passports.begin(), passports.end(), p1_check) << '\n'
              << std::count_if(passports.begin(), passports.end(), p2_check) << std::endl;

    return 0;
}

#endif // DAY_4
