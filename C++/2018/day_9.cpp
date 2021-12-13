#ifndef DAY_9
#define DAY_9

#include <algorithm>
#include <iostream>
#include <vector>

struct node {
    std::size_t val;
    node* prev;
    node* next;
};

void link(node* first, node* second) {
    first->next = second;
    second->prev = first;
}

std::size_t remove(node* n) {
    std::size_t val = n->val;
    link(n->prev, n->next);
    delete n;
    return val;
}

int day_9_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::size_t num_players = std::stoul(argv[1]);
    std::size_t max_marbles = std::stoul(argv[2]);

    node* curr = new node;
    curr->val = 0;
    curr->prev = curr;
    curr->next = curr;

    std::vector<std::size_t> score(num_players, 0UL);

    for (std::size_t i = 1; i <= max_marbles; ++i) {
        if (i % 23 == 0) {
            score[i % num_players] += i;
            for (std::size_t j = 0; j < 6; ++j)
                curr = curr->prev;
            score[i % num_players] += remove(curr->prev);
        } else {
            curr = curr->next;
            node* next = new node{i, curr, curr->next};
            curr->next->prev = next;
            curr->next = next;
            curr = next;
        }
    }

    std::cout << *std::max_element(score.begin(), score.end()) << std::endl;

    return 0;
}

#endif // DAY_9
