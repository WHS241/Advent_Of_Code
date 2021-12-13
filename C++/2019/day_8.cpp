#ifndef DAY_8
#define DAY_8 2

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int day_8_main(int argc, char** argv) {
    if (argc < 4)
        return 1;
    std::size_t dimX, dimY;
    dimX = std::stoul(argv[2]);
    dimY = std::stoul(argv[3]);

    std::ifstream in_read(argv[1]);
    std::string input;
    std::getline(in_read, input);
    std::vector<std::size_t> parsed;
    for (char c : input)
        parsed.push_back(c - '0');

    std::size_t pixels_per_layer = dimX * dimY;
    std::size_t num_layers = parsed.size() / pixels_per_layer;

    std::vector<std::vector<std::size_t>> layered;
    for (std::size_t i = 0; i < num_layers; ++i)
        layered.emplace_back(parsed.begin() + i * pixels_per_layer,
                             parsed.begin() + (i + 1) * pixels_per_layer);

#if DAY_8 == 1
    std::vector<std::size_t> num_zeroes(layered.size());
    std::transform(
      layered.begin(), layered.end(), num_zeroes.begin(),
      [](const std::vector<std::size_t>& x) { return std::count(x.begin(), x.end(), 0); });
    auto min_zero_it = std::min_element(num_zeroes.begin(), num_zeroes.end());
    const std::vector<std::size_t>& min_zero_layer = layered[min_zero_it - num_zeroes.begin()];
    std::cout << std::count(min_zero_layer.begin(), min_zero_layer.end(), 1)
                   * std::count(min_zero_layer.begin(), min_zero_layer.end(), 2)
              << std::endl;
#elif DAY_8 == 2
    std::vector<std::size_t> generated_image(pixels_per_layer);
    for (std::size_t i = 0; i < generated_image.size(); ++i) {
        for (std::size_t j = 0; j < layered.size(); ++j) {
            if (layered[j][i] != 2) {
                generated_image[i] = layered[j][i];
                break;
            }
        }
    }

    const char *RESET = "\033[0m", *BLACK = "\033[30m", *WHITE = "\033[37m", *FILLER = "\u25A0";

    for (std::size_t i = 0; i < generated_image.size(); ++i) {
        if (i % dimX == 0)
            std::cout << std::endl;
        std::cout << (generated_image[i] ? WHITE : BLACK) << FILLER << RESET;
    }
    std::cout << std::endl;
#endif

    return 0;
}

#endif // DAY_8
