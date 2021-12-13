#ifndef UTIL_H
#define UTIL_H

#include <array>
#include <sstream>
#include <string>

#include <openssl/md5.h>

std::string hex_string_hash(const std::string& input, std::size_t num_stretches = 0) {
    std::string curr = input;
    for (std::size_t i = 0; i <= num_stretches; ++i) {
    std::ostringstream printer;
    printer << std::hex;

    std::array<unsigned char, MD5_DIGEST_LENGTH> hash;
    MD5((const unsigned char*) curr.c_str(), curr.size(), hash.data());

    for (unsigned char c : hash)
        printer << (int)(c / 0x10) << (int)(c % 0x10);

    curr = printer.str();
    }

    return curr;
}

#endif // UTIL_H
