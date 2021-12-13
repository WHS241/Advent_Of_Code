#ifndef DAY_1
#define DAY_1

#define DAY_1_PART 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>

long calculate_fuel(long weight) {
    return weight / 3 - 2;
}

int day_1_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream in_stream(argv[1]);
    std::list<long> inputs;
    std::string input;

    while (std::getline(in_stream, input)) {
        inputs.push_back(std::stoul(input));
    }

    std::transform(inputs.begin(), inputs.end(), inputs.begin(), [](long in) {
#if DAY_1_PART == 1
        return calculate_fuel(in);
#elif DAY_1_PART == 2
            long total = 0L;
            for (long curr = calculate_fuel(in); curr > 0; curr = calculate_fuel(curr))
                total += curr;
            return total;
#endif
    });

    std::cout << std::accumulate(inputs.begin(), inputs.end(), 0UL) << std::endl;

    return 0;
}

#endif // DAY_1
