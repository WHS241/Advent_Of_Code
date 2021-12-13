#ifndef DAY_22
#define DAY_22 2

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <utility>

#include <structures/heap>

constexpr std::size_t START_HP = 50;
constexpr std::size_t START_MP = 500;
constexpr std::size_t BOSS_HP = 55;
constexpr std::size_t BOSS_DMG = 8;

struct state {
    std::size_t player_HP;
    std::size_t player_MP;
    std::size_t boss_HP;
    std::array<std::size_t, 3> status_counters;
    bool player_turn;
};

state apply_status_and_boss_attack(state s) {
    if (!s.player_turn) {
        std::size_t damage = s.status_counters[0] ? 1 : BOSS_DMG;
        s.player_HP = std::max(s.player_HP, damage) - damage;
    }
#if DAY_22 == 2
    else if (s.player_HP) {
        --s.player_HP;
        if (!s.player_HP)
            return s;
    }
#endif
    if (s.status_counters[0])
        --s.status_counters[0];
    if (s.status_counters[1]) {
        s.boss_HP = std::max(s.boss_HP, 3UL) - 3;
        --s.status_counters[1];
    }
    if (s.status_counters[2]) {
        s.player_MP += 101;
        --s.status_counters[2];
    }
    return s;
}

int day_22_main(int, char**) {
    auto compare = [](const std::pair<state, std::size_t>& x,
                      const std::pair<state, std::size_t>& y) {
        return x.second < y.second;
    };

    heap::priority_queue<std::pair<state, std::size_t>, decltype(compare)> frontier(compare);
    frontier.insert(std::make_pair(state{START_HP, START_MP, BOSS_HP, {0, 0, 0}, true}, 0));

    std::size_t mana_used;
    while (!(&frontier)->empty()) {
        std::pair<state, std::size_t> curr = frontier.remove_root();
        state successor = apply_status_and_boss_attack(curr.first);
        successor.player_turn = !successor.player_turn;
        if (successor.boss_HP == 0) {
            mana_used = curr.second;
            break;
        } else if (successor.player_HP == 0) {
            continue;
        }

        if (curr.first.player_turn) {
            if (successor.player_MP >= 53) {
                state next(successor);
                next.player_MP -= 53;
                next.boss_HP = std::max(next.boss_HP, 4UL) - 4;
                frontier.insert(std::make_pair(next, curr.second + 53));
            }
            if (successor.player_MP >= 73) {
                state next(successor);
                next.player_MP -= 73;
                next.boss_HP = std::max(next.boss_HP, 2UL) - 2;
                next.player_HP += 2;
                frontier.insert(std::make_pair(next, curr.second + 73));
            }
            if (successor.player_MP >= 113 && !successor.status_counters[0]) {
                state next(successor);
                next.player_MP -= 113;
                next.status_counters[0] = 6;
                frontier.insert(std::make_pair(next, curr.second + 113));
            }
            if (successor.player_MP >= 173 && !successor.status_counters[1]) {
                state next(successor);
                next.player_MP -= 173;
                next.status_counters[1] = 6;
                frontier.insert(std::make_pair(next, curr.second + 173));
            }
            if (successor.player_MP >= 229 && !successor.status_counters[2]) {
                state next(successor);
                next.player_MP -= 229;
                next.status_counters[2] = 5;
                frontier.insert(std::make_pair(next, curr.second + 229));
            }
        } else {
            frontier.insert(std::make_pair(successor, curr.second));
        }
    }

    std::cout << mana_used << std::endl;

    return 0;
}

#endif // DAY_22
