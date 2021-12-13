#ifndef DAY_3
#define DAY_3 2

#include <iostream>
#include <vector>

int day_3_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::size_t target = std::stoul(argv[1]);

#if DAY_3 == 1
    int last_root = 0;
    while (last_root * last_root < target)
        ++last_root;
    --last_root;

    auto last_corner = last_root * last_root;
    int x, y;
    if (target % 2) {
        x = (last_root - 1) / 2 + std::min(1, last_root + last_corner - int(target));
        y = (1 - last_root) / 2 + std::min(int(target) - last_corner - 1, last_root);
    } else {
        x = -last_root / 2 + std::max(0, int(target) - last_root - last_corner - 1);
        y = last_root / 2 - std::min((int)target - last_corner - 1, last_root);
    }

    std::cout << std::abs(x) + std::abs(y) << std::endl;
#elif DAY_3 == 2
    std::vector<std::size_t> vals;
    vals.push_back(1);
    std::size_t n  = 1;
    std::size_t n2 = 1;

    while (vals.back() <= target) {
        // sweep clockwise
        vals.push_back(vals.back());

        std::size_t curr = vals.size();

        if (curr - n2 == 1 && n >= 2)
            vals.back() += vals[n2 - 4 * n + 4];
        else if (curr - n2 == 2 || curr - n2 == n + 2)
            vals.back() += vals[curr - 3];
        else if (curr - n2 <= n + 1 && n >= 2)
            vals.back() += vals[curr - 4 * n + 1];
        else if (n >= 2)
            vals.back() += vals[curr - 4 * n - 1];

        if (curr - n2 >= 2 && curr - n2 <= n)
            vals.back() += vals[curr - 4 * n + 2];
        else if (curr - n2 >= n + 2 && curr - n2 <= 2 * n + 1)
            vals.back() += vals[curr - 4 * n];

        if (curr - n2 >= 2 && curr - n2 <= n - 1)
            vals.back() += vals[curr - 4 * n + 3];
        else if (curr - n2 >= n + 2 && curr - n2 <= 2 * n)
            vals.back() += vals[curr - 4 * n + 1];

        if (curr - n2 == 2 * n + 1) {
            ++n;
            n2 = curr;
        }
    }

    std::cout << vals.back() << std::endl;
#endif

    return 0;
}

#endif // DAY_3
