#ifndef DAY_4
#define DAY_4
#include <algorithm>
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>

#include <openssl/md5.h>

bool has_leading_zeroes_md5(std::string input, std::size_t num_zeroes) {
    std::array<unsigned char, MD5_DIGEST_LENGTH> checksum;
    MD5((unsigned char*) input.c_str(), input.size(), checksum.data());
    bool output = std::all_of(checksum.begin(), checksum.begin() + num_zeroes / 2,
                              [](unsigned char c) { return (c == (unsigned char) 0); });
    if (num_zeroes % 2)
        output = output && (checksum[num_zeroes / 2] <= 0x10);
    return output;
}

int day_4_main(int argc, char** argv) {
    if (argc < 3)
        return 1;
    std::string input(argv[1]);
    std::size_t target_zeroes = std::stoul(argv[2]); // P1: argv[2] = "5"; P2: argv[2] = "6"

    std::size_t append_val = 0;
    while (!has_leading_zeroes_md5(input + std::to_string(append_val), target_zeroes))
        ++append_val;
    std::cout << append_val << std::endl; // Result

    // Print the hash itself
    unsigned char res[MD5_DIGEST_LENGTH];
    input += std::to_string(append_val);
    MD5((unsigned char*) input.c_str(), input.size(), res);
    std::cout << std::hex << std::setfill('0') << std::setw(4);
    std::copy_n(res, MD5_DIGEST_LENGTH, std::ostream_iterator<unsigned>(std::cout, ""));
    std::cout << std::endl;

    return 0;
}
#endif // DAY_4
