#ifndef DAY_14
#define DAY_14 2

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>

static const std::string START = "ORE";
static const std::string TARGET = "FUEL";
static const std::string SEPARATOR = "=>";
static const char DELIM = ',';

int day_14_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    // Key: Output; Value: { Reactant, Amount }
    std::unordered_map<std::string,
                       std::pair<std::size_t, std::list<std::pair<std::string, std::size_t>>>>
      reactions;

    std::ifstream reader(argv[1]);

    for (std::string line; std::getline(reader, line);) {
        std::size_t sep_pos = line.find(SEPARATOR);
        std::string reactants = line.substr(0, sep_pos);
        std::string product_info = line.substr(sep_pos + SEPARATOR.size());

        std::istringstream prod_read(product_info);
        std::string product;
        std::size_t amount;
        prod_read >> amount >> product;

        if (reactions.find(product) != reactions.end()) {
            std::cout << "ERROR: 已存在：" << product;
            return 1;
        }

        reactions[product].first = amount;

        std::list<std::pair<std::string, std::size_t>>& r_list = reactions[product].second;

        std::istringstream reactants_read(reactants);
        for (std::string entry; std::getline(reactants_read, entry, DELIM);) {
            std::string reactant;
            std::size_t amount;
            std::istringstream entry_read(entry);
            entry_read >> amount >> reactant;

            r_list.emplace_back(reactant, amount);
        }
    }

    // Topological sort
    std::unordered_map<std::string, std::size_t> in_degrees;
    for (const std::pair<const std::string,
                         std::pair<std::size_t, std::list<std::pair<std::string, std::size_t>>>>&
           v : reactions) {
        if (in_degrees.find(v.first) == in_degrees.end())
            in_degrees[v.first] = 0U;
        for (const std::pair<std::string, std::size_t>& x : v.second.second)
            ++in_degrees[x.first];
    }

    std::list<std::string> order;
    for (const std::pair<const std::string, std::size_t>& v : in_degrees)
        if (v.second == 0U)
            order.push_back(v.first);

    for (const std::string& mat : order) {
        if (reactions.find(mat) != reactions.end()) {
            for (const std::pair<std::string, std::size_t>& dep : reactions[mat].second) {
                if (--in_degrees[dep.first] == 0U) {
                    order.push_back(dep.first);
                }
            }
        }
    }

#if DAY_14 == 1
    std::unordered_map<std::string, std::size_t> amount_needed;
    amount_needed[TARGET] = 1;
    for (auto it = std::find(order.begin(), order.end(), TARGET); it != order.end(); ++it) {
        const std::string& curr = *it;
        if (curr == START)
            break;

        std::size_t batch_size = reactions[curr].first;
        std::size_t required = amount_needed[curr];

        std::size_t num_batches = required / batch_size;
        if (required % batch_size)
            ++num_batches;

        for (const std::pair<std::string, std::size_t>& dep : reactions[curr].second)
            amount_needed[dep.first] += num_batches * dep.second;
    }
    std::cout << amount_needed[START] << std::endl;
#elif DAY_14 == 2
    std::size_t reservoir_size = 1000000000000UL;
    std::size_t lower = 0UL;
    std::size_t upper = reservoir_size;

    while (lower + 1 < upper) {
        std::size_t check_amount = (lower + upper) / 2;
        std::unordered_map<std::string, std::size_t> amount_needed;
        amount_needed[TARGET] = check_amount;

        for (auto it = std::find(order.begin(), order.end(), TARGET); it != order.end(); ++it) {
            const std::string& curr = *it;
            if (curr == START)
                break;

            std::size_t batch_size = reactions[curr].first;
            std::size_t required = amount_needed[curr];

            std::size_t num_batches = required / batch_size;
            if (required % batch_size)
                ++num_batches;

            for (const std::pair<std::string, std::size_t>& dep : reactions[curr].second)
                amount_needed[dep.first] += num_batches * dep.second;
        }

        if (amount_needed[START] <= reservoir_size)
            lower = check_amount;
        else if (amount_needed[START] > reservoir_size)
            upper = check_amount;

        if (amount_needed[START] == reservoir_size)
            break;
    }

    std::cout << lower << std::endl;
#endif

    return 0;
}

#endif // DAY_14
