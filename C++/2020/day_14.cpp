#ifndef DAY_14
#define DAY_14 2

#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <stdexcept>
#include <unordered_map>

constexpr std::size_t MASK_SIZE = 36;

#if DAY_14 == 1
std::size_t
#elif DAY_14 == 2
std::list<std::size_t>
#endif
  apply_mask(const std::string& mask, std::size_t x) {
    std::bitset<MASK_SIZE> in(x);
    std::bitset<MASK_SIZE> out;

#if DAY_14 == 2
    std::list<std::size_t> edit_pos;
#endif

    std::size_t i = 0;
    for (auto it = mask.rbegin(); it != mask.rend(); ++it) {
#if DAY_14 == 1
        if (*it == 'X')
            out[i] = in[i];
        else
            out[i] = *it - '0';
#elif DAY_14 == 2
        if (*it == 'X')
            edit_pos.push_back(i);
        else if (*it == '0')
            out[i] = in[i];
        else if (*it == '1')
            out[i] = true;
#endif

        ++i;
    }

#if DAY_14 == 1
    return out.to_ulong();
#elif DAY_14 == 2
    std::list<bool> settings;
    while (settings.size() != edit_pos.size())
        settings.push_back(false);

    std::list<std::size_t> output;

    bool finished = false;
    while (!finished) {
        auto it = edit_pos.begin();
        auto it2 = settings.begin();
        std::bitset<MASK_SIZE> v(out);
        while (it != edit_pos.end())
            v[*it++] = *it2++;

        output.push_back(v.to_ulong());

        finished = true;
        for (bool& b : settings) {
            b = !b;
            if (b) {
                finished = false;
                break;
            }
        }
    }

    return output;
#endif
}

int day_14_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unordered_map<std::size_t, std::size_t> memory;

    std::string mask;

    for (std::string line; std::getline(reader, line);) {
        if (line[1] == 'a') { // mask
            mask = line.substr(line.find('=') + 2);
            if (mask.size() != MASK_SIZE)
                throw std::domain_error("大小不对");
        } else {
            std::size_t address = std::stoul(line.substr(4, line.find(']') - 4));
            std::size_t value = std::stoul(line.substr(line.find('=') + 2));
#if DAY_14 == 1
            memory[address] = apply_mask(mask, value);
#elif DAY_14 == 2
            for (std::size_t x : apply_mask(mask, address))
                memory[x] = value;
#endif
        }
    }

    std::cout << std::transform_reduce(
      memory.begin(), memory.end(), 0UL, std::plus<std::size_t>(),
      std::mem_fn(&std::pair<const std::size_t, std::size_t>::second))
              << std::endl;

    return 0;
}

#endif // DAY_14
