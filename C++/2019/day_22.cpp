#ifndef DAY_22
#define DAY_22 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "big_int.h"

struct transform {
    big_int multiple;
    big_int increment;
    big_int base;

    big_int operator()(big_int in) const { return (multiple * in + increment) % base; }

    transform operator*(transform rhs) const {
        rhs.multiple = (multiple * rhs.multiple) % base;
        rhs.increment = (rhs.increment * multiple + increment) % base;
        return rhs;
    }
};

transform pow(const transform& b, big_int exp) {
    transform r;
    r.increment = 0;
    r.multiple = 1;
    r.base = b.base;

    for (char c : exp.to_string(2)) {
        r = r * r;
        if (c == '1')
            r = r * b;
    }

    return r;
}

transform cut(transform t, big_int cut_in) {
    t.increment = (t.increment - cut_in) % t.base;
    return t;
}

transform deal(transform t, big_int cut_in) {
    t.multiple = (t.multiple * cut_in) % t.base;
    t.increment = (t.increment * cut_in) % t.base;
    return t;
}

transform deal(transform t) {
    t = deal(t, -1);
    t = cut(t, 1);
    return t;
}

transform invert(transform t) {
    big_int orig_inc = t.increment;
    t = pow(t, t.base - 2);
    t.increment = (-orig_inc * t.multiple) % t.base;

    return t;
}

transform get_transform(const std::vector<std::string>& instructions, std::size_t deck_size) {
    transform output;
    output.increment = 0;
    output.multiple = 1;
    output.base = deck_size;
    for (const std::string& line : instructions) {
        if (line[0] == 'c') {
            int index = std::stoi(line.substr(4));
            output = cut(output, index);
        } else if (line[5] == 'i') {
            output = deal(output);
        } else if (line[5] == 'w') {
            output = deal(output, std::stoul(line.substr(20)));
        }
    }

    return output;
}

int day_22_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t deck_size = std::stoul(argv[2]);

    std::vector<std::string> instructions;
    for (std::string line; std::getline(reader, line);)
        instructions.push_back(line);

    transform t = get_transform(instructions, deck_size);
    std::cout << t(2019) << std::endl;

    if (argc < 4)
        return 0;

    // PART 2
    std::size_t exp = std::stoul(argv[3]);
    t = invert(pow(t, exp));
    std::cout << t(2020) << std::endl;

    return 0;
}

#endif // DAY_22
