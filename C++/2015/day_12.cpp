#ifndef DAY_12
#define DAY_12

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>

#include "json.h"

JSON_node* parse_JSON(std::istream& is) {
    char c = is.get();
    while (std::isspace(c))
        c = is.get();
    std::unique_ptr<JSON_node> curr;
    if (c == '{') {
        curr.reset(new JSON_object());
        auto tmp = dynamic_cast<JSON_object*>(curr.get());
        c = is.peek();
        if (c == '}')
            c = is.get();
        while (c != '}') {
            std::string field;
            {
                std::unique_ptr<JSON_node> f_v(parse_JSON(is));
                auto tmp2 = dynamic_cast<JSON_string*>(f_v.get());
                if (!tmp2)
                    throw tmp2;
                field = tmp2->value;
            }
            while (c != ':')
                c = is.get();
            if (!tmp->fields.emplace(field, parse_JSON(is)).second)
                throw "Duplicate";
            c = is.get();
            while (std::isspace(c))
                c = is.get();
        }
    } else if (c == '[') {
        curr.reset(new JSON_array());
        auto tmp = dynamic_cast<JSON_array*>(curr.get());
        c = is.peek();
        if (c == ']')
            c = is.get();
        while (c != ']') {
            tmp->members.push_back(parse_JSON(is));
            c = is.get();
            while (std::isspace(c))
                c = is.get();
            if (c != ']' && c != ',')
                throw c;
        }
    } else if (c == '"') {
        curr.reset(new JSON_string());
        auto tmp = dynamic_cast<JSON_string*>(curr.get());
        c = is.get();
        while (c != '"') {
            tmp->value.push_back(c);
            c = is.get();
        }
    } else {
        curr.reset(new JSON_num());
        bool negative = c == '-';
        if (negative)
            c = is.get();
        auto tmp = dynamic_cast<JSON_num*>(curr.get());
        while (std::isdigit(c)) {
            tmp->value = tmp->value * 10 + (c - '0');
            c = is.get();
        }
        is.unget();
        if (negative)
            tmp->value *= -1;
    }
    return curr.release();
}

int sum_nums(const JSON_node* node, const std::string& ignore = "") {
    if (dynamic_cast<const JSON_num*>(node))
        return dynamic_cast<const JSON_num*>(node)->value;
    if (dynamic_cast<const JSON_string*>(node))
        return 0;
    if (dynamic_cast<const JSON_array*>(node)) {
        auto p = dynamic_cast<const JSON_array*>(node);
        return std::transform_reduce(
          p->members.begin(), p->members.end(), 0, std::plus<>(),
          std::bind(&sum_nums, std::placeholders::_1, std::cref(ignore)));
    }
    if (dynamic_cast<const JSON_object*>(node)) {
        auto p = dynamic_cast<const JSON_object*>(node);
        if (!ignore.empty()
            && std::count_if(p->fields.begin(), p->fields.end(), [&ignore](const auto& f) {
                   return dynamic_cast<const JSON_string*>(f.second)
                       && dynamic_cast<const JSON_string*>(f.second)->value == ignore;
               }))
            return 0;

        return std::transform_reduce(
          p->fields.begin(), p->fields.end(), 0, std::plus<>(),
          [&ignore](const auto& f) { return sum_nums(f.second, ignore); });
    }
    return 0;
}

int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::unique_ptr<JSON_node> input(parse_JSON(reader));
    std::cout << sum_nums(input.get()) << '\n' // Part 1
              << sum_nums(input.get(), "red")  // Part 2
              << std::endl;

    return 0;
}

#endif // DAY_12
