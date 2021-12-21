#ifndef DAY_21
#define DAY_21

#include <algorithm>
#include <iostream>
#include <unordered_set>

int day_21_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::size_t player_1_pos = std::stoul(argv[1]), player_2_pos = std::stoul(argv[2]);

    std::size_t i = 0;
    std::size_t player_1_score = 0, player_2_score = 0;
    while (player_1_score < 1000 && player_2_score < 1000) {
        if (i % 2) {
            player_2_pos = (player_2_pos + 3 * (i + 2)) % 10;
            player_2_score += player_2_pos;
            if (player_2_pos == 0)
                player_2_score += 10;
        } else {
            player_1_pos = (player_1_pos + 3 * (i + 2)) % 10;
            player_1_score += player_1_pos;
            if (player_1_pos == 0)
                player_1_score += 10;
        }
        i += 3;
    }

    std::cout << i * std::min(player_1_score, player_2_score) << '\n';

    std::size_t num_pos[31][31][10][10]
                       [2]; // num_pos[w][x][y][z][t] = num timelines where: P1 at pos y, P2 at pos
                            // z, P1 with score w, P2 with score x, player t to move
    for (std::size_t i = 0; i < 31; ++i)
        for (std::size_t j = 0; j < 31; ++j)
            for (std::size_t k = 0; k < 10; ++k)
                for (std::size_t l = 0; l < 10; ++l)
                    for (std::size_t m = 0; m < 2; ++m)
                        num_pos[i][j][k][l][m] = 0;
    num_pos[0][0][std::stoul(argv[1]) % 10][std::stoul(argv[2]) % 10][1] = 1;
    for (std::size_t total_score = 0; total_score < 42; ++total_score) {
        for (std::size_t w = 0; w < 21; ++w) {
            std::size_t x = total_score - w;
            if (x < 21) {
                for (std::size_t y = 0; y < 10; ++y) {
                    for (std::size_t z = 0; z < 10; ++z) {
                        if (num_pos[w][x][y][z][0] || num_pos[w][x][y][z][1]) {
                            for (std::size_t a = 1; a <= 3; ++a) {
                                for (std::size_t b = 1; b <= 3; ++b) {
                                    for (std::size_t c = 1; c <= 3; ++c) {
                                        std::size_t dist_to_move = a + b + c;

                                        // P1 moves
                                        std::size_t p1_next = (y + dist_to_move) % 10;
                                        std::size_t p1_next_score = w + p1_next;
                                        if (!p1_next)
                                            p1_next_score += 10;
                                        num_pos[p1_next_score][x][p1_next][z][0] +=
                                          num_pos[w][x][y][z][1];

                                        // P2 moves
                                        std::size_t p2_next = (z + dist_to_move) % 10;
                                        std::size_t p2_next_score = x + p2_next;
                                        if (!p2_next)
                                            p2_next_score += 10;
                                        num_pos[w][p2_next_score][y][p2_next][1] +=
                                          num_pos[w][x][y][z][0];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::size_t player_1_total = 0;
    std::size_t player_2_total = 0;
    for (std::size_t w = 0; w < 31; ++w) {
        for (std::size_t x = 0; x < 31; ++x) {
            for (std::size_t y = 0; y < 10; ++y) {
                for (std::size_t z = 0; z < 10; ++z) {
                    if (w >= 21)
                        player_1_total += num_pos[w][x][y][z][0];
                    if (x >= 21)
                        player_2_total += num_pos[w][x][y][z][1];
                }
            }
        }
    }

    std::cout << player_1_total << ',' << player_2_total << std::endl;

    return 0;
}

#endif // DAY_21
