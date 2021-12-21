#ifndef DAY_18
#define DAY_18

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

struct snailfish {
    bool is_val;
    long val;
    std::unique_ptr<snailfish> left;
    std::unique_ptr<snailfish> right;
    snailfish* parent;
};

std::ostream& operator<<(std::ostream& os, const snailfish& num) {
    if (num.is_val)
        return os << num.val;
    return os << '[' << *num.left << ',' << *num.right << ']';
}

std::pair<snailfish*, std::size_t> build(const std::string& s, std::size_t start) {
    std::unique_ptr<snailfish> root(new snailfish());
    root->parent = nullptr;
    if (s[start] != '[') {
        root->is_val = true;
        std::size_t end = s.find(s[start - 1] == '[' ? ',' : ']', start);
        root->val = std::stol(s.substr(start, end - start));
        return std::make_pair(root.release(), end - start);
    }

    auto left_res = build(s, start + 1);
    root->left.reset(left_res.first);
    root->left->parent = root.get();
    auto right_res = build(s, start + left_res.second + 2);
    root->right.reset(right_res.first);
    root->right->parent = root.get();
    return std::make_pair(root.release(), left_res.second + right_res.second + 3);
}

std::unique_ptr<snailfish> merge(std::unique_ptr<snailfish>& lhs, std::unique_ptr<snailfish>& rhs) {
    std::unique_ptr<snailfish> result(new snailfish());
    result->is_val = false;
    result->left = std::move(lhs);
    result->right = std::move(rhs);
    result->left->parent = result->right->parent = result.get();
    return result;
}

snailfish* can_explode(std::unique_ptr<snailfish>& num, std::size_t level) {
    if (num->is_val)
        return nullptr;
    snailfish* left_reduce = can_explode(num->left, level + 1);
    if (left_reduce)
        return left_reduce;
    snailfish* right_reduce = can_explode(num->right, level + 1);
    if (right_reduce)
        return right_reduce;
    return (level >= 4) ? num.get() : nullptr;
}

bool split(std::unique_ptr<snailfish>& num) {
    if (num->is_val) {
        if (num->val < 10)
            return false;
        num->is_val = false;
        num->left.reset(new snailfish());
        num->left->is_val = true;
        num->left->val = num->val / 2;
        num->right.reset(new snailfish());
        num->right->is_val = true;
        num->right->val = num->val - num->left->val;
        num->left->parent = num->right->parent = num.get();
        return true;
    }
    return split(num->left) || split(num->right);
}

bool reduce_once(std::unique_ptr<snailfish>& num) {
    snailfish* explode = can_explode(num, 0);
    if (explode) {
        snailfish* current = explode;
        while (current->parent && current->parent->left.get() == current)
            current = current->parent;
        if (current->parent) {
            current = current->parent->left.get();
            while (!current->is_val)
                current = current->right.get();
            current->val += explode->left->val;
        }

        current = explode;
        while (current->parent && current->parent->right.get() == current)
            current = current->parent;
        if (current->parent) {
            current = current->parent->right.get();
            while (!current->is_val)
                current = current->left.get();
            current->val += explode->right->val;
        }

        explode->left.reset();
        explode->right.reset();
        explode->is_val = true;
        explode->val = 0;

        return true;
    }
    return split(num);
}

long mag(std::unique_ptr<snailfish>& num) {
    return num->is_val ? num->val : (3 * mag(num->left) + 2 * mag(num->right));
}

int day_18_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::string s;
    std::getline(reader, s);

    std::vector<std::string> inputs{s};

    std::unique_ptr<snailfish> curr(build(s, 0).first);
    while (reduce_once(curr))
        ;
    while (std::getline(reader, s)) {
        inputs.push_back(s);
        std::unique_ptr<snailfish> next(build(s, 0).first);
        curr = merge(curr, next);
        while (reduce_once(curr))
            ;
    }

    std::cout << mag(curr) << '\n';

    long part_2 = std::numeric_limits<long>::min();
    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        for (auto it2 = inputs.begin(); it2 != inputs.end(); ++it2) {
            if (it != it2) {
                std::unique_ptr<snailfish> lhs(build(*it, 0).first);
                std::unique_ptr<snailfish> rhs(build(*it2, 0).first);
                lhs = merge(lhs, rhs);
                while (reduce_once(lhs))
                    ;
                part_2 = std::max(part_2, mag(lhs));
            }
        }
    }
    std::cout << part_2 << std::endl;

    return 0;
}

#endif // DAY_18
