#ifndef DAY_16
#define DAY_16

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

std::size_t parse_val(const std::vector<bool>& bits, std::size_t start, std::size_t length) {
    std::size_t result = 0;
    for (std::size_t i = 0; i < length; ++i) {
        result *= 2;
        result += bits[start + i];
    }
    return result;
}

struct packet {
    std::size_t version_id;
    std::size_t type_id;
    std::size_t literal;
    std::vector<packet> subpackets;
};

std::pair<std::size_t, packet> extract_packet(const std::vector<bool>& bits) {
    packet result;
    result.version_id = parse_val(bits, 0, 3);
    result.type_id = parse_val(bits, 3, 3);
    result.literal = 0;

    std::size_t num_bits;

    if (result.type_id == 4) {
        for (std::size_t i = 6; i < bits.size(); i += 5) {
            result.literal = 16 * result.literal + parse_val(bits, i + 1, 4);
            if (!bits[i]) {
                num_bits = i + 5;
                break;
            }
        }
    } else if (bits[6]) {
        std::size_t num_subpackets = parse_val(bits, 7, 11);
        num_bits = 18;
        for (std::size_t i = 0; i < num_subpackets; ++i) {
            std::vector<bool> sub(bits.begin() + num_bits, bits.end());
            auto sub_parsed = extract_packet(sub);
            result.subpackets.push_back(sub_parsed.second);
            num_bits += sub_parsed.first;
        }
    } else {
        std::size_t subpackets_length = parse_val(bits, 7, 15);
        std::size_t processed = 0;
        while (processed < subpackets_length) {
            std::vector<bool> sub(bits.begin() + processed + 22, bits.end());
            auto sub_parsed = extract_packet(sub);
            result.subpackets.push_back(sub_parsed.second);
            processed += sub_parsed.first;
        }
        num_bits = processed + 22;
    }

    return std::make_pair(num_bits, result);
}

std::size_t part_1(const packet& p) {
    return p.version_id
         + std::transform_reduce(p.subpackets.begin(), p.subpackets.end(), 0UL, std::plus<>(),
                                 part_1);
}

std::size_t part_2(const packet& p) {
    switch (p.type_id) {
    case 0:
        return std::transform_reduce(p.subpackets.begin(), p.subpackets.end(), 0UL, std::plus<>(),
                                     part_2);

    case 1:
        return std::transform_reduce(p.subpackets.begin(), p.subpackets.end(), 1UL,
                                     std::multiplies<>(), part_2);

    case 2:
        return std::transform_reduce(
          p.subpackets.begin() + 1, p.subpackets.end(), part_2(p.subpackets.front()),
          [](std::size_t x, std::size_t y) { return std::min(x, y); }, part_2);

    case 3:
        return std::transform_reduce(
          p.subpackets.begin() + 1, p.subpackets.end(), part_2(p.subpackets.front()),
          [](std::size_t x, std::size_t y) { return std::max(x, y); }, part_2);

    case 4:
        return p.literal;

    case 5:
        return part_2(p.subpackets[0]) > part_2(p.subpackets[1]);

    case 6:
        return part_2(p.subpackets[0]) < part_2(p.subpackets[1]);

    case 7:
        return part_2(p.subpackets[0]) == part_2(p.subpackets[1]);

    default:
        break;
    }
    return 0;
}

int day_16_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::string s;
    std::getline(reader, s);

    std::vector<bool> bits(4 * s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        short val = (s[i] <= '9') ? (s[i] - '0') : (s[i] - 'A' + 10);
        bits[4 * i] = val / 8;
        bits[4 * i + 1] = (val / 4) % 2;
        bits[4 * i + 2] = (val / 2) % 2;
        bits[4 * i + 3] = val % 2;
    }

    packet p = extract_packet(bits).second;

    std::cout << part_1(p) << '\n' << part_2(p) << std::endl;

    return 0;
}

#endif // DAY_16
