#ifndef DAY_19
#define DAY_19 2

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "day_16.cpp"

static std::unordered_map<
  std::string,
  std::function<std::size_t(std::size_t, std::size_t, const std::array<std::size_t, 6>&)>>
  functions = {{"addi", addi<6>}, {"addr", addr<6>}, {"muli", muli<6>}, {"mulr", mulr<6>},
               {"bani", bani<6>}, {"banr", banr<6>}, {"bori", bori<6>}, {"setr", setr<6>},
               {"seti", seti<6>}, {"gtir", gtir<6>}, {"gtri", gtri<6>}, {"gtrr", gtrr<6>},
               {"eqir", eqir<6>}, {"eqri", eqri<6>}, {"eqrr", eqrr<6>}};

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::tuple<std::string, std::size_t, std::size_t, std::size_t>> code;
    std::array<std::size_t, 6> registers;
    std::fill(registers.begin(), registers.end(), 0UL);
    std::size_t* ip;

#if DAY_19 == 2
    ++registers.front();
#endif

    std::string line;
    std::getline(reader, line);
    ip = registers.data() + std::stoul(line.substr(4));

    while (std::getline(reader, line)) {
        std::istringstream parser(line);
        std::string instr;
        std::size_t a, b, c;
        parser >> instr >> a >> b >> c;
        code.emplace_back(instr, a, b, c);
    }

    while (*ip != 1) {
        auto [instr, a, b, c] = code[*ip];
        registers[c] = (functions.at(instr))(a, b, registers);
        ++*ip;
    }

    std::cout << "Find the sum of all divisors of " << registers[5] << std::endl;

    return 0;
}

#endif // DAY_19
