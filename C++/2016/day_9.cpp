#ifndef DAY_9
#define DAY_9

#include <fstream>
#include <iostream>
#include <limits>

std::size_t
  decompressed_length(std::istream& is, bool recurse,
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

            if (recurse) {
                result += reps * decompressed_length(is, recurse, length);
            } else {
                result += reps * length;
                is.ignore(length);
            }
        }
    }

    return result;
}

int day_9_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::string input;
    std::getline(reader, input);
    reader.seekg(0);

    std::cout << decompressed_length(reader, false) << '\n';
    reader = std::ifstream(argv[1]);
    std::cout << decompressed_length(reader, true) << std::endl;

    return 0;
}

#endif // DAY_9
