#ifndef DAY_5
#define DAY_5

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

bool is_nice_p1(const std::string& input) {
    const std::unordered_set<char> vowels({'a', 'e', 'i', 'o', 'u'});
    const std::unordered_set<char> danger({'a', 'c', 'p', 'x'});
    char last = input[0];
    std::size_t vowel_count = vowels.count(input[0]);
    bool found_double = false;

    for (std::size_t i = 1; i < input.size(); ++i) {
        if (danger.find(last) != danger.end() && input[i] == last + 1)
            return false;
        if (input[i] == last)
            found_double = true;
        vowel_count += vowels.count(input[i]);
        last = input[i];
    }

    return vowel_count >= 3 && found_double;
}

bool is_nice_p2(const std::string& input) {
    std::unordered_set<std::string> distincts;
    std::unordered_map<char, std::size_t> repeated;

    bool repeated_pair = false;
    bool sep_rep = false;

    for (std::size_t i = 1; i < input.size() && !(repeated_pair && sep_rep); ++i) {
        if (input[i - 1] == input[i]) {                    // double letters
            if (repeated.find(input[i]) == repeated.end()) // insert if first occurrence
                repeated[input[i]] = i - 1;
            else if (repeated[input[i]] != i - 2) // no overlap
                repeated_pair = true;
        } else { // different letters in pair
            std::string pair_sub = input.substr(i - 1, 2);
            if (distincts.find(pair_sub) != distincts.end())
                repeated_pair = true;
            distincts.insert(pair_sub); // insert if first occurrence
        }

        if (i >= 2 && input[i - 2] == input[i])
            sep_rep = true;
    }

    return repeated_pair && sep_rep;
}

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_nice_1(0), num_nice_2(0);
    for (std::string line; std::getline(reader, line);) {
        if (is_nice_p1(line))
            ++num_nice_1;

        if (is_nice_p2(line))
            ++num_nice_2;
    }

    std::cout << num_nice_1 << '\n' << num_nice_2 << std::endl;
    return 0;
}

#endif // DAY_5
