#ifndef DAY_16
#define DAY_16 2

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <util/pair_hash.h>

#include <structures/graph.h>

#include <graph/bipartite.h>

template<std::size_t REG_SIZE>
std::size_t addr(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] + registers[b];
}

template<std::size_t REG_SIZE>
std::size_t addi(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] + b;
}

template<std::size_t REG_SIZE>
std::size_t mulr(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] * registers[b];
}

template<std::size_t REG_SIZE>
std::size_t muli(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] * b;
}

template<std::size_t REG_SIZE>
std::size_t banr(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] & registers[b];
}

template<std::size_t REG_SIZE>
std::size_t bani(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] & b;
}

template<std::size_t REG_SIZE>
std::size_t borr(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] | registers[b];
}

template<std::size_t REG_SIZE>
std::size_t bori(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] | b;
}

template<std::size_t REG_SIZE>
std::size_t setr(std::size_t a, std::size_t, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a];
}

template<std::size_t REG_SIZE>
std::size_t seti(std::size_t a, std::size_t, const std::array<std::size_t, REG_SIZE>&) {
    return a;
}

template<std::size_t REG_SIZE>
std::size_t gtir(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return a > registers[b];
}

template<std::size_t REG_SIZE>
std::size_t gtri(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] > b;
}

template<std::size_t REG_SIZE>
std::size_t gtrr(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] > registers[b];
}

template<std::size_t REG_SIZE>
std::size_t eqir(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return a == registers[b];
}

template<std::size_t REG_SIZE>
std::size_t eqri(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] == b;
}

template<std::size_t REG_SIZE>
std::size_t eqrr(std::size_t a, std::size_t b, const std::array<std::size_t, REG_SIZE>& registers) {
    return registers[a] == registers[b];
}

std::array<std::function<std::size_t(std::size_t, std::size_t, const std::array<std::size_t, 4>&)>,
           16>
  ops = {addr<4>, addi<4>, mulr<4>, muli<4>, banr<4>, bani<4>, borr<4>, bori<4>,
         setr<4>, seti<4>, gtir<4>, gtri<4>, gtrr<4>, eqir<4>, eqri<4>, eqrr<4>};

int day_16_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

#if DAY_16 == 1
    std::size_t count = 0;
#elif DAY_16 == 2
    std::array<std::array<bool, 16>, 16> valid;
    for (std::size_t i = 0; i < valid.size(); ++i)
        for (std::size_t j = 0; j < valid.size(); ++j)
            valid[i][j] = true;
#endif

    for (std::string line; std::getline(reader, line);) {
        if (line.empty() || std::isdigit(line.front()))
            break;
        std::array<std::size_t, 4> input, output, instr;
        std::istringstream parser(line);

        parser.ignore(10, '[');
        for (std::size_t i = 0; i < input.size(); ++i) {
            parser >> input[i];
            parser.ignore(1);
        }

        std::getline(reader, line);
        parser = std::istringstream(line);

        for (std::size_t i = 0; i < instr.size(); ++i) {
            parser >> instr[i];
        }

        std::getline(reader, line);
        parser = std::istringstream(line);
        parser.ignore(10, '[');
        for (std::size_t i = 0; i < output.size(); ++i) {
            parser >> output[i];
            parser.ignore(1);
        }

        std::getline(reader, line);

#if DAY_16 == 1
        if (std::count_if(ops.begin(), ops.end(),
                          [&input, &output, &instr](auto f) {
                              return output[instr[3]] == f(instr[1], instr[2], input);
                          })
            >= 3)
            ++count;
#elif DAY_16 == 2
        for (std::size_t i = 0; i < ops.size(); ++i)
            if (output[instr[3]] != ops[i](instr[1], instr[2], input))
                valid[instr[0]][i] = false;
#endif
    }

#if DAY_16 == 1
    std::cout << count << std::endl;
#elif DAY_16 == 2
    graph::unweighted_graph<std::pair<bool, std::size_t>, false, util::pair_hash<bool, std::size_t>>
      valid_matches;
    for (std::size_t i = 0; i < valid.size(); ++i) {
        valid_matches.add_vertex({true, i});
        valid_matches.add_vertex({false, i});
    }

    for (std::size_t i = 0; i < valid.size(); ++i)
        for (std::size_t j = 0; j < valid.size(); ++j)
            if (valid[i][j])
                valid_matches.force_add({true, i}, {false, j});

    auto matches = graph_alg::maximum_bipartite_matching(valid_matches);

    std::array<std::size_t, 16> valids;
    for (auto p : matches) {
        if (p.first.first)
            valids[p.first.second] = p.second.second;
        else
            valids[p.second.second] = p.first.second;
    }

    std::array<std::size_t, 4> registers = {0, 0, 0, 0};
    for (std::string line; std::getline(reader, line);) {
        std::array<std::size_t, 4> op;
        std::istringstream parse(line);
        std::copy(std::istream_iterator<std::size_t>(parse), std::istream_iterator<std::size_t>(),
                  op.begin());
        registers[op[3]] = ops[valids[op[0]]](op[1], op[2], registers);
    }

    for (std::size_t x : registers)
        std::cout << x << std::endl;
#endif

    return 0;
}

#endif // DAY_16
