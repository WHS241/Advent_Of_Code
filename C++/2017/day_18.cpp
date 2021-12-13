#ifndef DAY_18
#define DAY_18 2

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

long get_value(std::unordered_map<char, long>& registers, const std::string& s) {
    return std::isalpha(s.front()) ? registers[s.front()] : std::stoi(s);
}

#if DAY_18 == 2
std::mutex wait_lock;
bool waiting[2] = {false, false};
std::list<long> queues[2];

void run_machine(const std::vector<std::vector<std::string>>& instructions, long pid) {
    std::unordered_map<char, long> registers;
    registers['p'] = pid;

    std::size_t num_sent = 0;
    for (std::size_t i = 0; i < instructions.size();) {
        if (instructions[i].front() == "snd") {
            std::lock_guard<std::mutex> guard(wait_lock);
            long to_send = get_value(registers, instructions[i][1]);
            queues[1 - pid].push_back(to_send);
            ++num_sent;
            ++i;
        } else if (instructions[i].front() == "set") {
            registers[instructions[i][1][0]] = get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "add") {
            registers[instructions[i][1][0]] += get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "mul") {
            registers[instructions[i][1][0]] *= get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "mod") {
            registers[instructions[i][1][0]] %= get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "rcv") {
            bool found_v = false;
            while (!found_v) {
                {
                    std::lock_guard<std::mutex> guard(wait_lock);
                    if (!queues[pid].empty()) {
                        found_v      = true;
                        waiting[pid] = false;
                        break;
                    }
                    waiting[pid] = true;
                    if (waiting[1 - pid] && queues[1 - pid].empty()) {
                        std::cout << pid << ": " << num_sent << std::endl;
                        return;
                    }
                }

                using namespace std::chrono_literals;
                std::this_thread::sleep_for(500ms);
            }

            registers[instructions[i][1][0]] = queues[pid].front();
            queues[pid].pop_front();

            ++i;
        } else if (instructions[i].front() == "jgz") {
            bool jump = get_value(registers, instructions[i][1]) > 0;
            if (jump)
                i += get_value(registers, instructions[i][2]);
            else
                ++i;
        }
    }
}
#endif

int day_18_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    std::vector<std::vector<std::string>> instructions;

    for (std::string line; std::getline(reader, line);) {
        std::istringstream line_read(line);

        instructions.emplace_back();
        std::copy(std::istream_iterator<std::string>(line_read),
                  std::istream_iterator<std::string>(), std::back_inserter(instructions.back()));
    }

#if DAY_18 == 1
    std::unordered_map<char, long> registers;

    for (std::size_t i = 0; i < instructions.size();) {
        if (instructions[i].front() == "snd") {
            std::cout << get_value(registers, instructions[i][1]) << std::endl;
            ++i;
        } else if (instructions[i].front() == "set") {
            registers[instructions[i][1][0]] = get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "add") {
            registers[instructions[i][1][0]] += get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "mul") {
            registers[instructions[i][1][0]] *= get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "mod") {
            registers[instructions[i][1][0]] %= get_value(registers, instructions[i][2]);
            ++i;
        } else if (instructions[i].front() == "rcv") {
            if (registers[instructions[i][1][0]])
                break;
            ++i;
        } else if (instructions[i].front() == "jgz") {
            bool jump = registers[instructions[i][1][0]] > 0;
            if (jump)
                i += get_value(registers, instructions[i][2]);
            else
                ++i;
        }
    }
#elif DAY_18 == 2
    std::thread t1(run_machine, std::ref(instructions), 0);
    std::thread t2(run_machine, std::ref(instructions), 1);

    t1.join();
    t2.join();
#endif

    return 0;
}

#endif // DAY_18
