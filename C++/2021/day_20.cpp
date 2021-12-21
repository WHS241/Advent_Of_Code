#ifndef DAY_20
#define DAY_20

#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

std::vector<std::vector<bool>> enhance(const std::bitset<512>& enhancer,
                                       const std::vector<std::vector<bool>>& input,
                                       bool fuzzy_background) {
    std::vector<std::vector<bool>> output(input.size() + 2);
    for (std::size_t i = 0; i < output.size(); ++i) {
        output[i].resize(input[0].size() + 2, false);
        for (std::size_t j = 0; j < output[i].size(); ++j) {
            std::bitset<9> region;
            region[8] = (i >= 2 && j >= 2) ? input[i - 2][j - 2] : fuzzy_background;
            region[7] =
              (i >= 2 && j >= 1 && j <= input[0].size()) ? input[i - 2][j - 1] : fuzzy_background;
            region[6] = (i >= 2 && j < input.size()) ? input[i - 2][j] : fuzzy_background;
            region[5] =
              (i >= 1 && i <= input.size() && j >= 2) ? input[i - 1][j - 2] : fuzzy_background;
            region[4] = (i >= 1 && i <= input.size() && j >= 1 && j <= input[0].size())
                        ? input[i - 1][j - 1]
                        : fuzzy_background;
            region[3] = (i >= 1 && i <= input.size() && j < input.size()) ? input[i - 1][j]
                                                                          : fuzzy_background;
            region[2] = (i < input.size() && j >= 2) ? input[i][j - 2] : fuzzy_background;
            region[1] = (i < input.size() && j >= 1 && j <= input[0].size()) ? input[i][j - 1]
                                                                             : fuzzy_background;
            region[0] = (i < input.size() && j < input.size()) ? input[i][j] : fuzzy_background;

            output[i][j] = enhancer[region.to_ulong()];
        }
    }
    return output;
}

int day_20_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::ifstream reader(argv[1]);
    std::size_t num_it = std::stoul(argv[2]);

    std::string enhancer, line;

    std::getline(reader, enhancer);
    std::getline(reader, line);

    std::reverse(enhancer.begin(), enhancer.end());
    std::bitset<512> enhance_bits(enhancer, 0, enhancer.size(), '.', '#');

    std::vector<std::vector<bool>> image;
    while (std::getline(reader, line)) {
        image.emplace_back(line.size());
        for (std::size_t i = 0; i < line.size(); ++i)
            image.back()[i] = line[i] == '#';
    }

    bool fuzzy = enhance_bits[0];

    for (std::size_t i = 0; i < num_it; ++i)
        image = enhance(enhance_bits, image, i % 2 && fuzzy);
    std::cout << std::transform_reduce(
      image.begin(), image.end(), 0UL, std::plus<>(), [](const std::vector<bool>& r) {
          return std::count_if(r.begin(), r.end(), std::identity());
      }) << std::endl;

    return 0;
}

#endif // DAY_20
