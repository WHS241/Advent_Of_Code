#ifndef DAY_14
#define DAY_14 2

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int day_14_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::string recipes = "37";
    std::size_t i(0), j(1);

#if DAY_14 == 1
    std::size_t num_recipes = std::stoul(argv[1]);
    constexpr std::size_t OUT_SIZE = 10;

    while (recipes.size() < num_recipes + OUT_SIZE)
#elif DAY_14 == 2
    std::size_t last_search = 0;
    std::string target(argv[1]);

    while (recipes.find(target, last_search) == std::string::npos)
#endif
    {
        std::size_t temp = last_search;
        last_search = recipes.size();
        for (std::size_t i = 1; i < target.size(); ++i)
            if (recipes.find(target.substr(0, i), temp) == recipes.size() - i)
                last_search = recipes.size() - i;
        std::size_t next = (recipes[i] - '0') + (recipes[j] - '0');
        recipes += std::to_string(next);
        i = (i + (recipes[i] - '0') + 1) % recipes.size();
        j = (j + (recipes[j] - '0') + 1) % recipes.size();
    }

#if DAY_14 == 1
    std::cout << recipes.substr(num_recipes, OUT_SIZE) << std::endl;
#elif DAY_14 == 2
    std::cout << recipes.find(argv[1], last_search) << std::endl;
#endif

    return 0;
}

#endif // DAY_14
