#ifndef DAY_24
#define DAY_24

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <list>
#include <unordered_set>

constexpr std::size_t LEVEL_DIM = 5;
constexpr std::size_t CENTER = LEVEL_DIM / 2;
typedef std::array<std::array<bool, LEVEL_DIM>, LEVEL_DIM> level;

std::list<level> advance(const std::list<level>& curr) {
    std::list<level> next(curr);

    auto read_it = curr.begin();
    auto write_it = next.begin();
    while (read_it != curr.end()) {
        for (std::size_t i = 0; i < LEVEL_DIM; ++i) {
            for (std::size_t j = 0; j < LEVEL_DIM; ++j) {
                if (i != CENTER || j != CENTER) {
                    std::size_t num_neighbors = 0;
                    if (i == 0) {
                        auto read2 = read_it;
                        if (read2 != curr.begin()) {
                            --read2;
                            if ((*read2)[CENTER - 1][CENTER])
                                ++num_neighbors;
                        }
                    } else if (i == CENTER + 1 && j == CENTER) {
                        auto read2 = read_it;
                        ++read2;
                        if (read2 != curr.end())
                            for (std::size_t k = 0; k < LEVEL_DIM; ++k)
                                if ((*read2)[LEVEL_DIM - 1][k])
                                    ++num_neighbors;
                    } else if ((*read_it)[i - 1][j]) {
                        ++num_neighbors;
                    }
                    if (i == LEVEL_DIM - 1) {
                        auto read2 = read_it;
                        if (read2 != curr.begin()) {
                            --read2;
                            if ((*read2)[CENTER + 1][CENTER])
                                ++num_neighbors;
                        }
                    } else if (i == CENTER - 1 && j == CENTER) {
                        auto read2 = read_it;
                        ++read2;
                        if (read2 != curr.end())
                            for (std::size_t k = 0; k < LEVEL_DIM; ++k)
                                if ((*read2)[0][k])
                                    ++num_neighbors;
                    } else if ((*read_it)[i + 1][j]) {
                        ++num_neighbors;
                    }
                    if (j == 0) {
                        auto read2 = read_it;
                        if (read2 != curr.begin()) {
                            --read2;
                            if ((*read2)[CENTER][CENTER - 1])
                                ++num_neighbors;
                        }
                    } else if (i == CENTER && j == CENTER + 1) {
                        auto read2 = read_it;
                        ++read2;
                        if (read2 != curr.end())
                            for (std::size_t k = 0; k < LEVEL_DIM; ++k)
                                if ((*read2)[k][LEVEL_DIM - 1])
                                    ++num_neighbors;
                    } else if ((*read_it)[i][j - 1]) {
                        ++num_neighbors;
                    }
                    if (j == LEVEL_DIM - 1) {
                        auto read2 = read_it;
                        if (read2 != curr.begin()) {
                            --read2;
                            if ((*read2)[CENTER][CENTER + 1])
                                ++num_neighbors;
                        }
                    } else if (i == CENTER && j == CENTER - 1) {
                        auto read2 = read_it;
                        ++read2;
                        if (read2 != curr.end())
                            for (std::size_t k = 0; k < LEVEL_DIM; ++k)
                                if ((*read2)[k][0])
                                    ++num_neighbors;
                    } else if ((*read_it)[i][j + 1]) {
                        ++num_neighbors;
                    }

                    (*write_it)[i][j] =
                      (num_neighbors == 1) || (!(*read_it)[i][j] && (num_neighbors == 2));
                }
            }
        }
        ++read_it;
        ++write_it;
    }

    // New outer level
    level outermost = curr.front();
    level new_outer;
    std::fill(new_outer.begin(), new_outer.end(),
              std::array<bool, 5>{false, false, false, false, false});
    std::size_t num_outer = std::count(outermost.front().begin(), outermost.front().end(), true);
    new_outer[CENTER - 1][CENTER] = num_outer && (num_outer < 3);
    num_outer = 0;
    for (std::size_t i = 0; i < LEVEL_DIM && num_outer < 3; ++i)
        if (outermost[i][0])
            ++num_outer;
    new_outer[CENTER][CENTER - 1] = num_outer && (num_outer < 3);

    num_outer = std::count(outermost.back().begin(), outermost.back().end(), true);
    new_outer[CENTER + 1][CENTER] = num_outer && (num_outer < 3);
    num_outer = 0;
    for (std::size_t i = 0; i < LEVEL_DIM && num_outer < 3; ++i)
        if (outermost[i][LEVEL_DIM - 1])
            ++num_outer;
    new_outer[CENTER][CENTER + 1] = num_outer && (num_outer < 3);
    if (new_outer[CENTER - 1][CENTER] || new_outer[CENTER + 1][CENTER]
        || new_outer[CENTER][CENTER - 1] || new_outer[CENTER][CENTER + 1])
        next.push_front(new_outer);

    // New inner level
    level innermost = curr.back();
    if (innermost[CENTER - 1][CENTER] || innermost[CENTER + 1][CENTER]
        || innermost[CENTER][CENTER - 1] || innermost[CENTER][CENTER + 1]) {
        level new_inner;
        std::fill(new_inner.begin(), new_inner.end(),
                  std::array<bool, 5>{false, false, false, false, false});
        if (innermost[CENTER - 1][CENTER])
            std::fill(new_inner.front().begin(), new_inner.front().end(), true);
        if (innermost[CENTER + 1][CENTER])
            std::fill(new_inner.back().begin(), new_inner.back().end(), true);
        if (innermost[CENTER][CENTER - 1])
            for (std::size_t k = 0; k < LEVEL_DIM; ++k)
                new_inner[k][0] = true;
        if (innermost[CENTER][CENTER + 1])
            for (std::size_t k = 0; k < LEVEL_DIM; ++k)
                new_inner[k][LEVEL_DIM - 1] = true;
        next.push_back(new_inner);
    }

    return next;
}

int day_24_main(int argc, char** argv) {
    if (argc < 3)
        return 1;
    std::ifstream reader(argv[1]);

    level board;
    for (std::size_t i = 0; i < LEVEL_DIM; ++i) {
        std::string line;
        std::getline(reader, line);
        std::transform(line.begin(), line.begin() + LEVEL_DIM, board[i].begin(),
                       [](char c) { return c == '#'; });
    }

    const std::size_t DURATION = std::stoul(argv[2]);
    std::list<level> state;
    state.push_back(board);
    for (std::size_t i = 0; i < DURATION; ++i) {
        state = advance(state);
    }

    std::size_t count = 0;
    for (const level& l : state) {
        // std::cout << '\n';

        for (const level::value_type& row : l) {
            for (bool c : row) {
                if (c)
                    ++count;
                // std::cout << (c ? '#' : '.');
            }
            // std::cout << '\n';
        }
    }

    std::cout << count << std::endl;

    return 0;
}

#endif // DAY_24
