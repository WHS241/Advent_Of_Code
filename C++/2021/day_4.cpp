#ifndef DAY_4
#define DAY_4

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

typedef std::array<std::array<std::size_t, 5>, 5> board;
typedef std::array<std::array<bool, 5>, 5> checker;

bool check_bingo(const checker& c) {
    for (std::size_t i = 0; i < c.size(); ++i) {
        if (std::all_of(c[i].begin(), c[i].end(), std::identity()))
            return true;
        if (std::all_of(c.begin(), c.end(), [&i](const auto& r) { return r[i]; }))
            return true;
    }
    /*
    bool diag = true;
    for (std::size_t i = 0; diag && i < c.size(); ++i)
        diag = c[i][i];
    if (diag)
        return true;

    diag = true;
    for (std::size_t i = 0; diag && i < c.size(); ++i)
        diag = c[i][c.size() - i - 1];
    return diag;
    */
    return false;
}

std::size_t win_score(std::size_t last_val, const board& b, const checker& c) {
    std::size_t sum_unmarked = 0;
    for (std::size_t i = 0; i < 5; ++i)
        for (std::size_t j = 0; j < 5; ++j)
            if (!c[i][j])
                sum_unmarked += b[i][j];

    return last_val * sum_unmarked;
}

int day_4_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::string line;
    std::getline(reader, line);
    std::istringstream parser(line);

    std::list<std::size_t> win_order;

    for (std::string s; std::getline(parser, s, ',');)
        win_order.push_back(std::stoul(s));

    std::vector<board> boards;

    std::size_t i = 0;
    while (std::getline(reader, line)) {
        if (line.empty()) {
            i = 0;
            boards.emplace_back();
        } else {
            std::istringstream parser(line);
            std::copy(std::istream_iterator<std::size_t>{parser},
                      std::istream_iterator<std::size_t>{}, boards.back().at(i++).begin());
        }
    }

    std::vector<checker> track(boards.size());
    std::generate(track.begin(), track.end(), []() {
        checker grid;
        for (auto& r : grid)
            std::fill(r.begin(), r.end(), false);

        return grid;
    });

    auto won_it = track.end();
    auto last_it = track.begin();
    std::size_t p1_winner_val;
    std::size_t curr;

    while (std::any_of(track.begin(), track.end(), std::not_fn(check_bingo))) {
        curr = win_order.front();
        win_order.pop_front();

        for (std::size_t b = 0; b < boards.size(); ++b)
            if (!check_bingo(track[b]))
                for (std::size_t i = 0; i < 5; ++i)
                    for (std::size_t j = 0; j < 5; ++j)
                        track[b][i][j] = track[b][i][j] || boards[b][i][j] == curr;

        if (won_it == track.end()) {
            won_it = std::find_if(track.begin(), track.end(), check_bingo);
            if (won_it != track.end())
                p1_winner_val = curr;
        }

        auto tmp = std::find_if_not(track.begin(), track.end(), check_bingo);
        if (tmp != track.end())
            last_it = tmp;
    }

    std::cout << win_score(p1_winner_val, boards[won_it - track.begin()], *won_it) << '\n'
              << win_score(curr, boards[last_it - track.begin()], *last_it) << std::endl;

    return 0;
}

#endif // DAY_4
