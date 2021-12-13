#ifndef DAY_9
#define DAY_9 2

#include <fstream>
#include <iostream>
#include <limits>

std::size_t
  decompressed_length(std::istream& is,
                      std::size_t input_length = std::numeric_limits<std::streamsize>::max()) {
    std::size_t result = 0;
    std::size_t extracted = 0;
    for (char c; extracted < input_length && is >> c; ++extracted) {
        if (c != '(') {
            ++result;
        } else {
            std::string marker;
            while (c != ')') {
                marker.push_back(c);
                is >> c;
                ++extracted;
            }

            std::size_t pos = marker.find('x');
            std::size_t length = std::stoul(marker.substr(1, pos - 1));
            std::size_t reps = std::stoul(marker.substr(pos + 1));

            extracted += length;

#if DAY_9 == 1
            result += reps * length;
            is.ignore(length);
#elif DAY_9 == 2
            result += reps * decompressed_length(is, length);
#endif
        }
    }

    return result;
}

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::cout << decompressed_length(reader) << std::endl;

    return 0;
}

#endif // DAY_9
