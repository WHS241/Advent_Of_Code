#ifndef DAY_5
#define DAY_5

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>

#include <openssl/md5.h>

constexpr std::size_t PASSWORD_LENGTH = 8;
constexpr std::size_t NUM_LEAD_ZEROES = 5;

bool has_leading_zeroes_md5(const std::array<unsigned char, MD5_DIGEST_LENGTH>& checksum) {
    bool output = std::all_of(checksum.begin(), checksum.begin() + NUM_LEAD_ZEROES / 2,
                              [](unsigned char c) { return (c == (unsigned char) 0); });
    if (NUM_LEAD_ZEROES % 2)
        output = output && (checksum[NUM_LEAD_ZEROES / 2] <= 0x10);
    return output;
}

int day_5_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::string prefix(argv[1]);

    std::array<short, PASSWORD_LENGTH> password_p2;
    std::size_t num_found_p1(0);
    std::array<bool, PASSWORD_LENGTH> found_p2;
    std::fill(found_p2.begin(), found_p2.end(), false);

    std::cout << std::hex;

    for (std::size_t i = 0; num_found_p1 < PASSWORD_LENGTH
                            || std::any_of(found_p2.begin(), found_p2.end(), std::logical_not());
         ++i) {
        std::string curr = prefix + std::to_string(i);
        std::array<unsigned char, MD5_DIGEST_LENGTH> checksum;
        MD5((unsigned char*) curr.c_str(), curr.size(), checksum.data());
        if (has_leading_zeroes_md5(checksum)) {
            if (num_found_p1 < PASSWORD_LENGTH) {
                // P1
                unsigned char after_check = checksum[NUM_LEAD_ZEROES / 2];
                std::cout << (short) (NUM_LEAD_ZEROES % 2 ? after_check % 0x10
                                                          : after_check / 0x10);
                ++num_found_p1;
            }

            // P2
            int pos = checksum[NUM_LEAD_ZEROES / 2];
            int value = checksum[(NUM_LEAD_ZEROES + 1) / 2];
            if (NUM_LEAD_ZEROES % 2) {
                pos %= 0x10;
                value /= 0x10;
            } else {
                pos /= 0x10;
                value %= 0x10;
            }

            if (pos < PASSWORD_LENGTH && !found_p2[pos]) {
                password_p2[pos] = value;
                found_p2[pos] = true;
            }
        }
    }

    std::cout << '\n';
    std::copy(password_p2.begin(), password_p2.end(), std::ostream_iterator<short>(std::cout, ""));
    std::cout << std::endl;

    return 0;
}

#endif // DAY_5
